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
import harbour.passilic 1.0

Dialog {
    id: dlg

    property var requester: null

    DialogHeader {
    }

    PasswordField {
        id: passwordField

        anchors {
            centerIn: parent
            left: parent.left
            right: parent.right
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
        }

        focus: true
        placeholderText: "Key passphrase"
        EnterKey.iconSource: "image://theme/icom-m-enter-accept"
        EnterKey.onClicked: dlg.accept()

    }

    onRejected: requester.cancel()
    onAccepted: requester.setPassphrase(passwordField.text)
}
