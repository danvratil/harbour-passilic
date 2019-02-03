import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    Image {
        source: "image://passImage/cover.png?" + Theme.primaryColor
        anchors.centerIn: parent

        width: parent.width - 2 * Theme.paddingLarge
        height: sourceSize.height * width / sourceSize.width
    }
}
