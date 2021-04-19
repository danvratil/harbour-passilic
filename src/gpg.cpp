#include "gpg.h"

#include <QStandardPaths>
#include <QProcess>
#include <QIODevice>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTimer>
#include <QtConcurrent>
#include <QFutureWatcher>

namespace {

struct GpgExecutable {
    GpgExecutable(const QString &path, int major, int minor)
        : path(path), major_version(major), minor_version(minor)
    {}
    QString path = {};
    int major_version = 0;
    int minor_version = 0;
};

GpgExecutable findGpgExecutable()
{
    auto gpgExe = QStandardPaths::findExecutable(QStringLiteral("gpg2"));
    if (gpgExe.isEmpty()) {
        gpgExe = QStandardPaths::findExecutable(QStringLiteral("gpg"));
    }

    QProcess process;
    process.start(gpgExe, {QStringLiteral("--version")}, QIODevice::ReadOnly);
    process.waitForFinished();
    const auto line = process.readLine();
    static const QRegularExpression rex(QStringLiteral("([0-9]+).([0-9]+).([0-9]+)"));
    const auto match = rex.match(QString::fromUtf8(line));

    return {gpgExe, match.captured(1).toInt(), match.captured(2).toInt()};
}

} // namespace

Gpg::GetKeyTrustTask *Gpg::getKeyTrust(const Key &key)
{
    return new GetKeyTrustTask(key);
}

Gpg::UpdateKeyTrustTask *Gpg::updateKeyTrust(const Key &key, Key::Trust trust)
{
    return new UpdateKeyTrustTask(key, trust);
}

Gpg::EncryptTask *Gpg::encrypt(const QString &file, const Key &key, const QString &content)
{
    return new EncryptTask(file, key, content);
}

Gpg::DecryptTask *Gpg::decrypt(const QString &file, const Key &key, const QString &passphrase)
{
    return new DecryptTask(file, key, passphrase);
}


Gpg::Task::Task(QObject *parent)
    : QObject(parent)
{
    QTimer::singleShot(0, this, &Task::start);
}

bool Gpg::Task::error() const
{
    return !mError.isNull();
}

QString Gpg::Task::errorString() const
{
    return mError;
}

void Gpg::Task::setError(const QString &error)
{
    mError = error;
}

void Gpg::Task::start()
{
    qDebug() << "Starting task" << this;
    auto future = QtConcurrent::run(this, &Task::run);
    auto *watcher = new QFutureWatcher<void>;
    connect(watcher, &QFutureWatcher<void>::finished, watcher, &QObject::deleteLater);
    connect(watcher, &QFutureWatcher<void>::finished, this, &Gpg::Task::finished);
    connect(watcher, &QFutureWatcher<void>::finished, this, &QObject::deleteLater);
    watcher->setFuture(future);
}

Gpg::GetKeyTrustTask::GetKeyTrustTask(const Key &key)
    : mKey(key)
{}

Gpg::Key::Trust Gpg::GetKeyTrustTask::trust() const
{
    return mTrust;
}

void Gpg::GetKeyTrustTask::run()
{
    const auto gpg = findGpgExecutable();
    QProcess process;
    process.setProgram(gpg.path);
    process.setArguments({QStringLiteral("--list-key \"%1\"").arg(mKey.id), QStringLiteral("--with-colons")});
    process.start(QIODevice::ReadOnly);
    process.waitForFinished();
    while (!process.atEnd()) {
        const auto line = process.readLine();
        const auto cols = line.split(':');
        if (cols.size() < 8) {
            continue;
        }
        if (cols[0] == "uid") {
            if (cols[1].isEmpty()) {
                mTrust = Key::Trust::Unknown;
            }
            switch (cols[1][0]) {
            case 'u':
                mTrust = Key::Trust::Ultimate;
                break;
            case 'f':
                mTrust = Key::Trust::Full;
                break;
            case 'm':
                mTrust = Key::Trust::Marginal;
                break;
            case 'n':
                mTrust = Key::Trust::Never;
                break;
            case '-':
            default:
                mTrust = Key::Trust::Unknown;
                break;
            }
            break;
        }
    }
}

Gpg::UpdateKeyTrustTask::UpdateKeyTrustTask(const Key &key, Key::Trust trust)
    : Task()
    , mKey(key)
    , mTrust(trust)
{}

void Gpg::UpdateKeyTrustTask::run()
{
    const auto gpg = findGpgExecutable();
    QProcess process;
    process.setProgram(gpg.path);
    process.setArguments({QStringLiteral("--command-fd=1"),
                          QStringLiteral("--status-fd=1"),
                          QStringLiteral("--batch"),
                          QStringLiteral("--edit-key"),
                          mKey.id,
                          QStringLiteral("trust")});
    process.start();
    process.waitForStarted();
    while (process.state() == QProcess::Running) {
        process.waitForReadyRead();
        const auto line = process.readLine();
        if (line == "[GNUPG:] GET_LINE edit_ownertrust.value\n") {
            process.write(QByteArray::number(static_cast<int>(mTrust)) + "\n");
            process.closeWriteChannel();
            break;
        }
    }

    process.waitForFinished();
}

Gpg::EncryptTask::EncryptTask(const QString &file, const Key &key, const QString &content)
    : mFile(file), mKey(key), mContent(content)
{}

void Gpg::EncryptTask::run()
{
    const auto gpg = findGpgExecutable();
    QProcess process;
    process.setProgram(gpg.path);
    process.setArguments({QStringLiteral("--quiet"),
                          QStringLiteral("--status-fd=1"),
                          QStringLiteral("--command-fd=1"),
                          QStringLiteral("--batch"),
                          QStringLiteral("--encrypt"),
                          QStringLiteral("--no-encrypt-to"),
                          QStringLiteral("-r %1").arg(mKey.id),
                          QStringLiteral("-o%1").arg(mFile)});
    process.start();
    process.waitForStarted();
    process.write(mContent.toUtf8());
    process.closeWriteChannel();
    process.waitForFinished();
    if (process.exitCode() != 0) {
        const auto err = process.readAllStandardError();
        qWarning() << "Failed to encrypt data:" << err;
        setError(QString::fromUtf8(err));
    }
}

Gpg::DecryptTask::DecryptTask(const QString &file, const Key &key, const QString &passphrase)
    : Task(), mFile(file), mPassphrase(passphrase), mKey(key)
{}

QString Gpg::DecryptTask::content() const
{
    return mContent;
}

void Gpg::DecryptTask::run()
{
    const auto gpg = findGpgExecutable();
    QProcess process;
    process.setProgram(gpg.path);
    process.setArguments({QStringLiteral("--quiet"),
                          QStringLiteral("--batch"),
                          QStringLiteral("--decrypt"),
                          QStringLiteral("--no-tty"),
                          QStringLiteral("--command-fd=1"),
                          QStringLiteral("--no-encrypt-to"),
                          QStringLiteral("--compress-algo=none"),
                          QStringLiteral("--passphrase-fd=0"),
                          QStringLiteral("--pinentry-mode=loopback"),
                          QStringLiteral("-r %1").arg(mKey.id),
                          mFile});
    process.start();
    process.waitForStarted();
    process.write(mPassphrase.toUtf8());
    process.closeWriteChannel();
    process.waitForFinished();
    if (process.exitCode() != 0) {
        const auto err = process.readAllStandardError();
        qWarning() << "Failed to decrypt data:" << err;
        setError(QString::fromUtf8(err));
    } else {
        mContent = QString::fromUtf8(process.readAllStandardOutput());
    }
}
