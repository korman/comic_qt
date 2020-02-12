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

                width: parent.width
                model: 0
                delegate: ItemDelegate {
                    id:dele
                    text: ComicMgr.currentOpenBook().chapterName(index)
                    width: parent.width

                    onClicked: {
                        listView.visible = false
                        ComicMgr.currentOpenBook().openChapter(index)
                        stackView.push("Chapter.qml")
                    }
                }

                Component.onCompleted: {
                    model = ComicMgr.currentOpenBook().chapterCount()
                }
            }
        }
    }
}
