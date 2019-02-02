import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import harbour.passilic 1.0

ApplicationWindow
{
    id: app

    PasswordSortProxyModel {
        id: passwordsModel

        dynamicSortFilter: true
        isSortLocaleAware: true
        sortCaseSensitivity: Qt.CaseInsensitive

        sourceModel: PasswordsModel {}
    }

    PasswordFilterModel {
        id: filterModel

        sourceModel: passwordsModel
    }

    Component {
        id: passwordsPage

        PasswordListPage {
            model: passwordsModel
            onFolderSelected: {
                var newPage = passwordsPage.createObject(app, {
                        "rootIndex": index,
                        "currentPath": currentPath + "/" + name
                });
                app.pageStack.push(newPage)
            }
        }
    }

    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: {
        app.pageStack.push(passwordsPage.createObject(app));
    }
}
