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

Page {

    Column {
        anchors {
            fill: parent
            leftMargin: Theme.horizontalPageMargin
            rightMargin: Theme.horizontalPageMargin
        }

        spacing: Theme.paddingMedium


        PageHeader {
            title: qsTr("About")
        }

        Row {
            spacing: Theme.paddingLarge

            Image {
                anchors {
                    verticalCenter: parent.verticalCenter
                }

                id: icon
                source: "../images/icon.png"
            }

            Column {
                spacing: Theme.paddingMedium

                Label {
                    text: qsTr("Passilic")
                    font.pixelSize: Theme.fontSizeExtraLarge
                }

                Label {
                    text: qsTr("Version %1").arg(Qt.application.version)
                    font.pixelSize: Theme.fontSizeSmall
                }

                Label {
                    text: qsTr("Author: Daniel Vrátil")
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }

        Label {
            text: qsTr("Homepage: <a href=\"%1\">Github</a>").arg("https://github.com/danvratil/harbour-passilic")
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            text: qsTr("License: GPLv3")
            font.pixelSize: Theme.fontSizeSmall
        }
    }
}
