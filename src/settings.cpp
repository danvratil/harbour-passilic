/*
 *   Copyright (C) 2019  Daniel Vrátil <dvratil@kde.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "settings.h"

#include <QStandardPaths>
#include <QDir>

std::unique_ptr<Settings> Settings::kInstance = {};

static const QString OldSettingsDir = QStringLiteral("Daniel Vrátil");
static const QString SettingsDir = QStringLiteral("harbour-passilic");
static const QString SettingsName = QStringLiteral("passilic");

Settings::Settings()
    : QObject()
{
    // Migrate config directory to new location
    const auto oldPath = QStandardPaths::locate(QStandardPaths::ConfigLocation,
                                                OldSettingsDir,
                                                QStandardPaths::LocateDirectory);
    const QDir newDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
                                                + QLatin1Char('/') + SettingsDir);
    if (!oldPath.isEmpty() && !newDir.exists()) {
        qDebug("Migrating Passilic configuration from %s to %s",
               qUtf8Printable(oldPath),
               qUtf8Printable(newDir.absolutePath()));
        QDir().rename(oldPath, newDir.absolutePath());
    }

    mSettings.reset(new QSettings(SettingsDir, SettingsName));
}

Settings *Settings::self()
{
    if (!kInstance) {
        kInstance.reset(new Settings);
    }
    return kInstance.get();
}

void Settings::destroy()
{
    kInstance.reset();
}

void Settings::save()
{
    mSettings->sync();
}

#define IMPLEMENT_OPTION(type, lc, uc, name, defValue) \
    void Settings::set##uc(type val) { \
        if (lc() != val) { \
            mSettings->setValue(QStringLiteral(name), val); \
            Q_EMIT lc##Changed(); \
        } \
    } \
    type Settings::lc() const { \
        return mSettings->value(QStringLiteral(name), defValue).value<type>(); \
    }

IMPLEMENT_OPTION(int, expirationTimeout, ExpirationTimeout, "expirationTimeout", 45)


#undef IMPLEMENT_OPTION
