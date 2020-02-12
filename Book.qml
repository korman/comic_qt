import QtQuick 2.12
import QtQuick.Controls 2.5
import Comic.Common 1.0

Item {
    id: bookView
    visible: true
    anchors.fill: parent

    StackView {
        id:stackView

        anchors.fill: parent

        ScrollView {
            anchors.fill: parent

            ListView {
                id:listView

                property int name: 10

                width: parent.width
                model: 0
                delegate: ItemDelegate {
                    text: ComicMgr.currentOpenBook().chapterName(index)
                    width: parent.width
                    onClicked: {
                        listView.visible = false
                        stackView.push("Book.qml")
                    }
                }

                Component.onCompleted: {
                    model = ComicMgr.currentOpenBook().chapterCount()
                }
            }
        }
    }
}
