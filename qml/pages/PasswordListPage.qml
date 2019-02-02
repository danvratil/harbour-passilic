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
                height: Theme.itemSizeSmall

                Row {

                    spacing: Theme.paddingMedium

                    anchors {
                        left: parent.left
                        leftMargin: Theme.horizontalPageMargin
                        right: parent.right
                        rightMargin: Theme.horizontalPageMargin
                        verticalCenter: parent.verticalCenter
                    }

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

                onClicked: {
                    if (model.type === PasswordsModel.FolderEntry) {
                        passwordListPage.folderSelected(delegateModel.modelIndex(index), model.name);
                    } else {
                        console.log("Password for " + model.name + " requested");
                    }
                }
            }
        }
    }
}
