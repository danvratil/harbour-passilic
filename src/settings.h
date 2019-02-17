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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include <memory>

#define DECLARE_OPTION(type, lc, uc) \
    private: Q_PROPERTY(type lc READ lc WRITE set##uc NOTIFY lc##Changed) \
    public: type lc() const; \
    public: void set##uc(type val); \
    Q_SIGNALS: void lc##Changed();


class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings *self();
    static void destroy();

    Q_INVOKABLE void save();

    DECLARE_OPTION(int, expirationTimeout, ExpirationTimeout)

private:
    explicit Settings();
    std::unique_ptr<QSettings> mSettings;

    static std::unique_ptr<Settings> kInstance;
};

#undef DECLARE_OPTION

#endif // SETTINGS_H
