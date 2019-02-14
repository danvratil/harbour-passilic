/*
 *   Copyright (C) 2018  Daniel Vrátil <dvratil@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "passwordprovider.h"
#include "settings.h"

#include <QProcess>
#include <QStandardPaths>
#include <QClipboard>
#include <QGuiApplication>

namespace {

static const auto PasswordTimeoutUpdateInterval = 100;

}

PasswordProvider::PasswordProvider(const QString &path, QObject *parent)
    : QObject(parent)
    , mPath(path)
{}


PasswordProvider::~PasswordProvider()
{
    if (mGpg) {
        mGpg->terminate();
        delete mGpg;
    }
}

bool PasswordProvider::isValid() const
{
    return !mPassword.isNull();
}

QString PasswordProvider::password() const
{
    return mPassword;
}

void PasswordProvider::setPassword(const QString &password)
{
    qGuiApp->clipboard()->setText(password, QClipboard::Clipboard);

    if (qGuiApp->clipboard()->supportsSelection()) {
        qGuiApp->clipboard()->setText(password, QClipboard::Selection);
    }

    mPassword = password;
    Q_EMIT validChanged();
    Q_EMIT passwordChanged();

    mTimeout = defaultTimeout();
    Q_EMIT timeoutChanged();
    mTimer.start();
}

void PasswordProvider::expirePassword()
{
    removePasswordFromClipboard(mPassword);

    mPassword.clear();
    mTimer.stop();
    Q_EMIT validChanged();
    Q_EMIT passwordChanged();

    // Delete the provider, it's no longer needed
    deleteLater();
}

void PasswordProvider::requestPassword()
{
    setError({});
    mPassword.clear();
    mTimer.stop();
    Q_EMIT validChanged();
    Q_EMIT passwordChanged();

    mTimer.setInterval(PasswordTimeoutUpdateInterval);
    connect(&mTimer, &QTimer::timeout,
            this, [this]() {
                mTimeout -= mTimer.interval();
                Q_EMIT timeoutChanged();
                if (mTimeout == 0) {
                    expirePassword();
                }
            });

    bool isGpg2 = true;
    auto gpgExe = QStandardPaths::findExecutable(QStringLiteral("gpg2"));
    if (gpgExe.isEmpty()) {
        gpgExe = QStandardPaths::findExecutable(QStringLiteral("gpg"));
        isGpg2 = false;
    }
    if (gpgExe.isEmpty()) {
        qWarning("Failed to find gpg or gpg2 executables");
        setError(tr("Failed to decrypt password: GPG is not available"));
        return;
    }

    QStringList args = { QStringLiteral("-d"),
                         QStringLiteral("--quiet"),
                         QStringLiteral("--yes"),
                         QStringLiteral("--compress-algo=none"),
                         QStringLiteral("--no-encrypt-to"),
                         QStringLiteral("--passphrase-fd=0"),
                         mPath };
    if (isGpg2) {
        args = QStringList{ QStringLiteral("--pinentry-mode=loopback"),
                            QStringLiteral("--batch"),
                            QStringLiteral("--use-agent") }
                + args;
    }

    mGpg = new QProcess;
    connect(mGpg, &QProcess::errorOccurred,
            this, [this, gpgExe](QProcess::ProcessError state) {
                if (state == QProcess::FailedToStart) {
                    qWarning("Failed to start %s: %s", qUtf8Printable(gpgExe), qUtf8Printable(mGpg->errorString()));
                    setError(tr("Failed to decrypt password: Failed to start GPG"));
                }
            });
    connect(mGpg, &QProcess::readyReadStandardOutput,
            this, [this]() {
                // We only read the first line, second line usually is a username
                setPassword(QString::fromUtf8(mGpg->readLine()).trimmed());
            });
    connect(mGpg, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, [this]() {
                const auto err = mGpg->readAllStandardError();
                if (mPassword.isEmpty()) {
                    if (err.isEmpty()) {
                        setError(tr("Failed to decrypt password"));
                    } else {
                        setError(tr("Failed to decrypt password: %1").arg(QString::fromUtf8(err)));
                    }
                }

                mGpg->deleteLater();
                mGpg = nullptr;
            });
    mGpg->setProgram(gpgExe);
    mGpg->setArguments(args);
    mGpg->start(QIODevice::ReadWrite);
}

int PasswordProvider::timeout() const
{
    return mTimeout;
}

int PasswordProvider::defaultTimeout() const
{
    return Settings::self()->expirationTimeout() * 1000;
}

QString PasswordProvider::error() const
{
    return mError;
}

bool PasswordProvider::hasError() const
{
    return !mError.isNull();
}

void PasswordProvider::setError(const QString &error)
{
    mError = error;
    Q_EMIT errorChanged();
}

void PasswordProvider::cancel()
{
    if (mGpg) {
        mGpg->terminate();
        delete mGpg;
    }
    setError(tr("Cancelled by user."));
}

void PasswordProvider::setPassphrase(const QString &passphrase)
{
    if (!mGpg) {
        qWarning("Called PasswordProvider::setPassphrase without active GPG process");
        return;
    }

    mGpg->write(passphrase.toUtf8());
    mGpg->closeWriteChannel();
}

void PasswordProvider::removePasswordFromClipboard(const QString &password)
{
    // Clear the WS clipboard itself
    const auto clipboard = qGuiApp->clipboard();
    if (clipboard->text() == password) {
        clipboard->clear();
    }
}

