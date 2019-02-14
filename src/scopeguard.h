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

#ifndef SCOPEGUARD_H
#define SCOPEGUARD_H

template<typename Func>
class ScopeGuard
{
public:
    ScopeGuard(Func &&func)
        : mFunc(std::move(func))
    {}

    ScopeGuard(ScopeGuard<Func> &&) = default;
    ScopeGuard<Func> &operator=(ScopeGuard<Func> &&) = default;
    ScopeGuard(const ScopeGuard<Func> &) = delete;
    ScopeGuard<Func> &operator=(const ScopeGuard<Func> &) = delete;

    ~ScopeGuard()
    {
        mFunc();
    }

private:
    Func mFunc;
};

template<typename Func>
ScopeGuard<Func> scopeGuard(Func &&func)
{
    return ScopeGuard<Func>(std::move(func));
}

#endif // SCOPEGUARD_H
