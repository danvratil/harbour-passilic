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
#include "gpg.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QDir>
#include <QDebug>

namespace {

static const auto PasswordTimeoutUpdateInterval = 100;


#define PASSWORD_STORE_DIR "PASSWORD_STORE_DIR"

}

PasswordProvider::PasswordProvider(const QString &path, QObject *parent)
    : QObject(parent)
    , mPath(path)
{}


PasswordProvider::~PasswordProvider()
{}

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
    setError(tr("Cancelled by user."));
}

void PasswordProvider::setPassphrase(const QString &passphrase)
{
    const QString root =  qEnvironmentVariableIsSet(PASSWORD_STORE_DIR)
            ? QString::fromUtf8(qgetenv(PASSWORD_STORE_DIR))
            : QStringLiteral("%1/.password-store").arg(QDir::homePath());
    QFile gpgIdFile(root + QStringLiteral("/.gpg-id"));
    if (!gpgIdFile.exists()) {
        qWarning() << "Missing .gpg-id file (" << gpgIdFile.fileName() << ")";
        return;
    }
    gpgIdFile.open(QIODevice::ReadOnly);
    const auto gpgId = QString::fromUtf8(gpgIdFile.readAll()).trimmed();
    gpgIdFile.close();

    auto *job = Gpg::decrypt(mPath, Gpg::Key{gpgId}, passphrase);
    connect(job, &Gpg::DecryptTask::finished,
            this, [this, job]() {
                if (job->error()) {
                    qWarning() << "Failed to decrypt password: " << job->errorString();
                    setError(job->errorString());
                    return;
                }

                const QStringList lines = job->content().split(QLatin1Char('\n'));
                if (lines.empty()) {
                    qWarning() << "Failed to decrypt password or file empty";
                    setError(tr("Failed to decrypt password"));
                } else {
                    setPassword(lines[0]);
                }
            });
}

void PasswordProvider::removePasswordFromClipboard(const QString &password)
{
    // Clear the WS clipboard itself
    const auto clipboard = qGuiApp->clipboard();
    if (clipboard->text() == password) {
        clipboard->clear();
    }
}

