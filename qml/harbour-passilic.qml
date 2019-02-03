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

import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import harbour.passilic 1.0

ApplicationWindow
{
    id: app

    PasswordSortProxyModel {
        id: passwordsModel

        dynamicSortFilter: true
        isSortLocaleAware: true
        sortCaseSensitivity: Qt.CaseInsensitive

        sourceModel: PasswordsModel {}
    }

    PasswordFilterModel {
        id: filterModel

        sourceModel: passwordsModel
    }

    Component {
        id: passphraseRequester

        PassphraseRequester {}
    }

    Component {
        id: passwordsPage

        PasswordListPage {
            model: passwordsModel
            onFolderSelected: {
                var newPage = passwordsPage.createObject(app, {
                        "rootIndex": index,
                        "currentPath": currentPath + "/" + name
                });
                app.pageStack.push(newPage)
            }
        }
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: {
        app.pageStack.push(passwordsPage.createObject(app));
    }
}
