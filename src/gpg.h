#ifndef GPG_H
#define GPG_H

#include <QObject>
#include <QVector>

namespace Gpg
{
class ListKeysTask;
class FindKeyTask;
class GetKeyTrustTask;
class UpdateKeyTrustTask;
class DecryptTask;
class EncryptTask;

struct Key {
    enum class Trust {
        Unknown = 1,
        Never = 2,
        Marginal = 3,
        Full = 4,
        Ultimate = 5
    };

    QString id;
};

GetKeyTrustTask *getKeyTrust(const Key &key);

UpdateKeyTrustTask *updateKeyTrust(const Key &key, Key::Trust trust);

DecryptTask *decrypt(const QString &file, const Key &key, const QString &passphrase);

EncryptTask *encrypt(const QString &data, const Key &key, const QString &file);


class Task : public QObject {
    Q_OBJECT
public:
    bool error() const;
    QString errorString() const;

Q_SIGNALS:
    void finished();

protected:
    explicit Task(QObject *parent = nullptr);

    virtual void run() = 0;

    void setError(const QString &error);

private Q_SLOTS:
    void start();

private:
    QString mError;
};

class GetKeyTrustTask : public Task {
    Q_OBJECT
    friend GetKeyTrustTask *Gpg::getKeyTrust(const Key &);
public:
    Gpg::Key::Trust trust() const;

protected:
    void run() override;

private:
    explicit GetKeyTrustTask(const Key &key);

    Key mKey;
    Key::Trust mTrust = Key::Trust::Never;
};

class UpdateKeyTrustTask : public Task {
    Q_OBJECT
    friend UpdateKeyTrustTask *Gpg::updateKeyTrust(const Key &, Key::Trust);
protected:
    void run() override;

private:
    UpdateKeyTrustTask(const Gpg::Key &key, Gpg::Key::Trust trust);

    Key mKey;
    Key::Trust mTrust = Key::Trust::Never;
};

class DecryptTask : public Task {
    Q_OBJECT
    friend DecryptTask *Gpg::decrypt(const QString &, const Key &, const QString &);
public:
    QString content() const;

protected:
    void run() override;

private:
    DecryptTask(const QString &file, const Key &key, const QString &passphrase);

    QString mFile;
    QString mPassphrase;
    Key mKey;
    QString mContent;
};

class EncryptTask : public Task {
    Q_OBJECT
    friend EncryptTask *Gpg::encrypt(const QString &, const Key &, const QString &);
protected:
    void run() override;

private:
    EncryptTask(const QString &file, const Key &key, const QString &content);

    QString mFile;
    Key mKey;
    QString mContent;
};

} // namespace Gpg

#endif // GPG_H
