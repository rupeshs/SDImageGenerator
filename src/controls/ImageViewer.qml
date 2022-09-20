import QtQuick 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.3

Image {
    property url folderPath
    property url currentImagePath

    id: imageViewer
    height: mainImage.height +lstView.height //+buttonOpen.height
    width: mainImage.width

    ColumnLayout{
        anchors.fill: parent

        Image{
            id: mainImage

            sourceSize.width: 450
            sourceSize.height: 450
            Layout.margins:  5
            opacity: 1
            source: currentImagePath
        }

        ListView {
            id: lstView

            width: 450
            height: 70
            orientation: ListView.Horizontal
            spacing: 10
            Layout.leftMargin:  10
            clip: true

            ScrollBar.horizontal:  ScrollBar {
                active: true
            }

            function initImageSource()
            {
                if (folderModel.count > 0) {
                    //const curIndex = folderModel.count -1;
                    const imageSource = folderModel.get(0 ,"filePath");
                    if (imageSource)
                        mainImage.source = "file:/" + imageSource;
                    //console.log("fffffffffffffffffffff",lstView.currentIndex)
                    //lstView.positionViewAtIndex(curIndex, ListView.End)
                    //lstView.positionViewAtEnd();
                    //lstView.currentIndex = curIndex;
                    //lstView.currentItem
                }
            }
            function changeCurrentImageSource()
            {
                if (folderModel.count > 0) {
                    const imageSource = folderModel.get(lstView.currentIndex ,"filePath");
                    if (imageSource)
                        mainImage.source = "file:/" + imageSource;
                }
            }

            FolderListModel {
                id: folderModel
                nameFilters: ["*.png","*.jpg"]
                folder: imageViewer.folderPath
                sortReversed: true
            }

            Component {
                id: fileDelegate
                Column {
                    id : columnId
                    opacity: ListView.isCurrentItem ? 1 : 0.3
                    Image {
                        width: 70
                        height: 70
                        fillMode: Image.PreserveAspectFit
                        smooth: true
                        source: fileURL
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {lstView.currentIndex = index;

                            }
                        }
                    }
                    Component.onCompleted: {
                        if (index == 0){
                            console.log("Load first image -> ",imageViewer.folderPath)
                            if (imageViewer.folderPath)
                                lstView.initImageSource();

                        }

                    }
                }

            }
            onCurrentIndexChanged: {
                if (imageViewer.folderPath)
                    lstView.changeCurrentImageSource();
            }
            model: folderModel
            delegate: fileDelegate
        }

    }
}
