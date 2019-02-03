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

#include "passwordsmodel.h"
#include "passwordfiltermodel.h"
#include "passwordsortproxymodel.h"
#include "imageprovider.h"

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif
#include <QScopedPointer>

#include <sailfishapp.h>

void addImageProvider(QQmlEngine *engine, const QString &id)
{
    engine->addImageProvider(id, new ImageProvider(id));
}

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());

    qmlRegisterType<PasswordsModel>("harbour.passilic", 1, 0, "PasswordsModel");
    qmlRegisterType<PasswordFilterModel>("harbour.passilic", 1, 0, "PasswordFilterModel");
    qmlRegisterType<PasswordSortProxyModel>("harbour.passilic", 1, 0, "PasswordSortProxyModel");

    addImageProvider(view->engine(), QStringLiteral("passIcon"));
    addImageProvider(view->engine(), QStringLiteral("passImage"));
    view->setSource(SailfishApp::pathToMainQml());
    view->show();

    return app->exec();
}
