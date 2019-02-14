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

std::unique_ptr<Settings> Settings::kInstance = {};

Settings::Settings(): QObject() {}

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
    mSettings.sync();
}

#define IMPLEMENT_OPTION(type, lc, uc, name, defValue) \
    void Settings::set##uc(type val) { \
        if (lc() != val) { \
            mSettings.setValue(QStringLiteral(name), val); \
            Q_EMIT lc##Changed(); \
        } \
    } \
    type Settings::lc() const { \
        return mSettings.value(QStringLiteral(name), defValue).value<type>(); \
    }

IMPLEMENT_OPTION(int, expirationTimeout, ExpirationTimeout, "expirationTimeout", 45)


#undef IMPLEMENT_OPTION
