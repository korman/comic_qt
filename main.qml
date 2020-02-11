import QtQuick 2.12
import QtQuick.Controls 2.5
import Comic.Common 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    StackView {
        id:stackView

        anchors.fill: parent

        Component {
              id: itemComponent

              Item {
                  anchors.fill: parent

                  Rectangle {
                      width:parent.width
                      height:parent.height

                      color: "#ff00ff"
                  }
              }
          }

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
                        listView.visible = false
                        stackView.push(itemComponent)
                    }
                }

                Component.onCompleted: {
                    ComicMgr.loadDir("/Users/kakuhiroshi/Downloads/漫画")

                    model = ComicMgr.bookCount()
                }
            }
        }
    }
}
