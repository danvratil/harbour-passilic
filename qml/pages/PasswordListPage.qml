import QtQuick 2.2
import QtQml.Models 2.2
import Sailfish.Silica 1.0
import harbour.passilic 1.0

Page {
    id: passwordListPage

    property var rootIndex: null
    property alias model: delegateModel.model
    property string currentPath: ""


    signal folderSelected(var index, var name)
    signal passwordRequested(var requester)


    SilicaListView {
        id: listView

        anchors.fill: parent

        header: PageHeader {
            id: pageHeader
            width: parent.width
            title: passwordListPage.currentPath === "" ? qsTr("Passilic") : passwordListPage.currentPath
        }

        model: DelegateModel {
            id: delegateModel

            rootIndex: passwordListPage.rootIndex

            delegate: ListItem {
                id: listItem

                property var password : null

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
                                            + ((model.type === PasswordsModel.FolderEntry) ? "icon-m-folder" : "icon-m-device-lock")
                                            + "?"
                                            + (listItem.highlighted ? Theme.highlightColor : Theme.primaryColor)
                                width: Theme.iconSizeSmall
                                height: width
                            }

                            Label {
                                id: label
                                text: model.name
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
                    if (model.type === PasswordsModel.FolderEntry) {
                        passwordListPage.folderSelected(delegateModel.modelIndex(index), model.name);
                    } else {
                        model.password.requestPassword()
                        var dialog = pageStack.push(Qt.resolvedUrl("PassphraseRequester.qml"),
                                                    { "requester": model.password })
                        dialog.done.connect(function() {
                            listItem.password = model.password
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
        }
    }
}
