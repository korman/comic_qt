import QtQuick 2.12
import QtQuick.Controls 2.5
import Comic.Common 1.0

Item {
    id: chapterView

    visible: true
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#ffffff"
    }

    ScrollView {
        anchors.fill: parent

        ListView {
            id:listView

            width: parent.width
            model: 0
            delegate: ItemDelegate {
                id:dele
                text: "aaa"
                width: parent.width

                ComicPainter {
                    id:item
                    anchors.fill: parent
                    Image {
                        id:img
                    }

                    Component.onCompleted: {
                        //                            var scale = img.height / img.width

                        //                            dele.width = parent.width
                        //                            dele.height = parent.width * scale

                        item.loadPage(index)
                        dele.width = item.imageWidth()
                        dele.height = item.imageHeight()
                    }
                }

                Component.onCompleted: {
                }
            }

            Component.onCompleted: {
                model = ComicMgr.currentOpenBook().currentChapter().chapterCount()
                footerToolbar.visible = true
            }
        }
    }
}
