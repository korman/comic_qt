import QtQuick 2.12
import QtQuick.Controls 2.5
import Comic.Common 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 960
    title: qsTr("Scroll")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1)
                {
                    stackView.pop()
                }
                else
                {
                    stackView.clear()
                }

                if (stackView.depth <= 2)
                {
                    footerToolbar.visible = false
                }
            }
        }

        Label {
            text: "hehe"
            anchors.centerIn: parent
        }
    }

    //    footer: ToolBar {
    //        id: footerToolbar
    //        contentHeight: toolButton.implicitHeight

    //        ToolButton {
    //            id: bottomButton
    //            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
    //            font.pixelSize: Qt.application.font.pixelSize * 1.6
    //            onClicked: {
    //            }
    //        }

    //        Component.onCompleted: {
    //            footerToolbar.visible = false
    //        }
    //    }

    StackView {
        id:stackView

        anchors.fill: parent

        ScrollView {
            anchors.fill: parent

            ListView {
                id:listView

                width: parent.width
                model: ComicMgr.bookCount()
                delegate: ItemDelegate {
                    text: ComicMgr.bookName(index)
                    width: parent.width
                    onClicked: {
                        ComicMgr.openBook(index)
                        stackView.push("Book.qml")
                    }
                }

                RemoteDelegate {
                    id:delegateBooks

                    onRequestMessage: {
                        listView.model = ComicMgr.bookCount()
                    }
                }

                Component.onCompleted: {
                    ComicMgr.setRemoteBookListCallback(delegateBooks)
               //     ComicMgr.loadDir("/Users/kakuhiroshi/Downloads/漫画")
                    ComicMgr.setBaseUrl("http://192.168.0.4:8081/")
                    ComicMgr.remoteLoadDir()

                    ComicMgr.setMaxWidth(parent.width)

                    model = 0
                }
            }
        }
    }
}
