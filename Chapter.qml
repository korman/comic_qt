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
        id: scrollView
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

            function nextPage() {
                console.log("clicked next page")
                model = 0

                ComicMgr.currentOpenBook().nextPage()

                model = ComicMgr.currentOpenBook().currentChapter().chapterCount()
            }

            function prePage() {
                console.log("clicked pre page")
                model = 0

                ComicMgr.currentOpenBook().prePage()

                model = ComicMgr.currentOpenBook().currentChapter().chapterCount()
            }

            Component.onCompleted: {
                model = ComicMgr.currentOpenBook().currentChapter().chapterCount()
                footerToolbar.visible = true
            }
        }
    }

    ToolBar {
        id: footerToolbar

        contentHeight: toolButton.implicitHeight
        width: parent.width
        x:0
        y:parent.height - toolButton.implicitHeight

        Rectangle {
            width: parent.width
            height: parent.height
            color: "#ffffff"
        }

        ToolButton {
            id: btnPreChapter
            text: "\u25C0"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                listView.prePage()
            }
        }

        ToolButton {
            id: btnNextChapter
            x: parent.width - btnPreChapter.width
            text: "\u25B6"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                listView.nextPage()
            }
        }
    }
}
