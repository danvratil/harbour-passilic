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
import harbour.passilic 1.0
import "../components"

Page {
    id: passwordListPage

    property var rootIndex: null
    property alias model: delegateModel.model
    property string currentPath: ""


    signal folderSelected(var index, var name)
    signal passwordRequested(var requester)


    Connections {
        target: model
        onModelReset: app.pageStack.pop(passwordListPage, PageStackAction.Immediate)
    }

    SilicaListView {
        id: listView

        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            width: parent.width
            title: passwordListPage.currentPath === "" ? qsTr("Passilic") : passwordListPage.currentPath
        }

        GlobalPullDownMenu {
            currentIndex: passwordListPage.rootIndex
            model: passwordListPage.model
        }

        model: DelegateModel {
            id: delegateModel

            rootIndex: passwordListPage.rootIndex

            delegate: PasswordDelegate {
                modelData: model

                onFolderSelected: {
                    passwordListPage.folderSelected(delegateModel.modelIndex(index), model.name);
                }
            }
        }

        VerticalScrollDecorator {
            flickable: listView
        }
    }
}
