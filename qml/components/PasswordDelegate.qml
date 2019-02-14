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

ListItem {
    id: listItem

    property var modelData
    property var password : null

    signal folderSelected()

    contentHeight: password === null ? Theme.itemSizeSmall : Theme.itemSizeLarge

    Row {
        anchors {
            fill: parent
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
            verticalCenter: parent.verticalCenter
            topMargin: Theme.paddingMedium
        }

        Column {
            spacing: Theme.paddingSmall
            width: parent.width

            Row {
                spacing: Theme.paddingMedium

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source:  "image://theme/"
                                + ((modelData.type === PasswordsModel.FolderEntry) ? "icon-m-folder" : "icon-m-device-lock")
                                + "?"
                                + (listItem.highlighted ? Theme.highlightColor : Theme.primaryColor)
                    width: Theme.iconSizeSmall
                    height: width
                }

                Label {
                    id: label
                    text: modelData.name
                }
            }

            Row {
                visible: password !== null
                width: parent.width

                Label {
                    id: errorLabel

                    visible: password !== null && password.hasError

                    text: password ? password.error : ""
                    font.pixelSize: Theme.fontSizeTiny
                }

                Label {
                    id: okLabel

                    visible: password !== null && password.valid

                    text: qsTr("Password copied to clipboard")
                    font.pixelSize: Theme.fontSizeTiny
                }
            }
        }
    }

    RemorseItem {
        id: remorse

        cancelText: qsTr("Expire password")

        // HACK: override RemorseItem._execute() to act as cancel when the timer expires
        function _execute(closeAfterExecute) {
            cancel()
        }

        onCanceled: {
            if (listItem.password) {
                listItem.password.expirePassword();
            }
        }
    }

    onClicked: {
        if (modelData.type === PasswordsModel.FolderEntry) {
            listItem.folderSelected()
        } else {
            modelData.password.requestPassword()
            var dialog = pageStack.push(Qt.resolvedUrl("../pages/PassphraseRequester.qml"),
                                        { "requester": modelData.password })
            dialog.done.connect(function() {
                listItem.password = modelData.password
                listItem.password.validChanged.connect(function() {
                    if (listItem.password.valid) {
                        remorse.execute(listItem, qsTr("Password will expire"),
                                        function() {
                                            if (listItem.password) {
                                                listItem.password.expirePassword();
                                            }
                                        }, listItem.password.defaultTimeout);
                    }
                });
            });
        }
    }
}
