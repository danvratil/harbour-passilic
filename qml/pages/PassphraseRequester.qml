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

        placeholderText: "Key passphrase"
        EnterKey.iconSource: "image://theme/icom-m-enter-accept"
        EnterKey.onClicked: dlg.accept()

    }

    onRejected: requester.cancel()
    onAccepted: requester.setPassphrase(passwordField.text)
}
