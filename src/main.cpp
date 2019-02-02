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

#include "passwordsmodel.h"
#include "passwordfiltermodel.h"
#include "passwordsortproxymodel.h"

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif
#include <QScopedPointer>

#include <sailfishapp.h>

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qmlRegisterType<PasswordsModel>("harbour.passilic", 1, 0, "PasswordsModel");
    qmlRegisterType<PasswordFilterModel>("harbour.passilic", 1, 0, "PasswordFilterModel");
    qmlRegisterType<PasswordSortProxyModel>("harbour.passilic", 1, 0, "PasswordSortProxyModel");

    view->setSource(SailfishApp::pathToMainQml());
    view->show();

    return app->exec();
}
