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
import QtQuick 2.0
import harbour.passilic 1.0

Dialog {
    id: settingsDialog

    onAccepted: {
        Settings.expirationTimeout = expirationSlider.sliderValue
        Settings.save()
    }

    DialogHeader {
        id: dialogHeader
        width: parent.width
    }

    SilicaFlickable {
        anchors {
            left: parent.left
            right: parent.right
            top: dialogHeader.bottom
            bottom: parent.bottom
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
        }

        Column {
            spacing: Theme.paddingMedium
            width: parent.width

            Label {
                text: qsTr("Password expiration")
            }
            Slider {
                id: expirationSlider
                width: parent.width

                minimumValue: 10
                maximumValue: 120
                stepSize: 1
                //: number of seconds
                valueText: qsTr("%1 s").arg(sliderValue)

                Component.onCompleted: value = Settings.expirationTimeout
            }
        }

        VerticalScrollDecorator {
            flickable: parent
        }
    }
}
