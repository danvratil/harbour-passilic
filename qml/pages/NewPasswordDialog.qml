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

Dialog {
    id: newPasswordDialog

    property var model
    property var currentIndex

    canAccept: nameField.text !== "" && passwordField.text !== ""

    onAccepted: {
        model.addPassword(currentIndex, nameField.text, passwordField.text, extrasField.text)
    }

    DialogHeader {
        id: header
        width: parent.width
    }

    SilicaFlickable {
        id: flickable

        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        clip: true
        contentHeight: column.height

        Column {
            id: column

            anchors {
                left: parent.left
                right: parent.right
                leftMargin: Theme.horizontalPageMargin
                rightMargin: Theme.horizontalPageMargin
            }

            spacing: Theme.paddingMedium

            Label {
                text: qsTr("Name:")
                width: parent.width
            }

            TextField {
                id: nameField
                width: parent.width
            }

            Label {
                text: qsTr("Password:")
                width: parent.width
            }

            PasswordField {
                id: passwordField
                width: parent.width
            }

            Button {
                id: generatePassButton
                width: parent.width
                text: qsTr("Generate Password")

                onClicked: app.pageStack.push(genPassDialog)
            }

            Label {
                text: qsTr("Additional Info:")
                width: parent.width
            }

            TextArea {
                id: extrasField
                width: parent.width
            }
        }


        HorizontalScrollDecorator {
            flickable: parent
        }
    }

    Component {
        id: genPassDialog

        GeneratePasswordDialog {
            onAccepted: {
                passwordField.text = PasswordGenerator.generate(passLen, allowSymbols)
            }
        }
    }
}
