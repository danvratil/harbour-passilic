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

import QtQuick 2.2
import QtQml.Models 2.2
import Sailfish.Silica 1.0

PullDownMenu {
    property var currentIndex
    property var model

    MenuItem {
        text: qsTr("About")
        onClicked: app.pageStack.push(Qt.resolvedUrl("../pages/AboutPage.qml"))
    }
    MenuItem {
        text: qsTr("Settings")
        onClicked: app.pageStack.push(Qt.resolvedUrl("../pages/SettingsPage.qml"))
    }
    MenuItem {
        text: qsTr("New Password")
        onClicked: app.pageStack.push(Qt.resolvedUrl("../pages/NewPasswordDialog.qml"),
                                      { "currentIndex": currentIndex,
                                        "model": model })
    }
    MenuItem {
        text: qsTr("Search")
        onClicked: app.pageStack.push(searchPage)
    }
}
