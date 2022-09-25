import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.folderlistmodel 2.15
import QtQuick.Layouts 1.3

RowLayout{
    property alias folderPath :folderModel.folder
    property int currentIndex : 0
    anchors.centerIn: parent

    function displayImage(){
        const imageSource = folderModel.get(currentIndex,"filePath");
        if (imageSource)
           mainImage.source = "file:/" + imageSource;
    }

    ToolButton{
        id : previousButton

        icon.source: "../images/previous.png"
        onClicked: {
            currentIndex -=1 ;
            displayImage()
            if (currentIndex == 0)
                previousButton.enabled = false;
            nextButton.enabled = true

        }
    }

    Image{
        id: mainImage
        sourceSize.width: 480
        sourceSize.height: 480
        Layout.margins:  1
        opacity: 1
        source: "../images/placeholder.png"
    }

    ToolButton{
        id : nextButton

        icon.source: "../images/next.png"
        font.bold: true
        onClicked: {
            const endIndex = folderModel.count - 1;
            if (currentIndex <= endIndex){
                currentIndex += 1 ;
                displayImage();
                if (currentIndex == endIndex)
                    nextButton.enabled = false;
            }
            else {
                currentIndex = endIndex;
            }
            previousButton.enabled = true;
        }
    }

    FolderListModel {
        id: folderModel

        nameFilters: ["*.png","*.jpg"]
        sortReversed: true

        onStatusChanged: {
            if (folderModel.status == FolderListModel.Ready){
                currentIndex = 0;
                if (folderModel.count === 1 ) {
                    displayImage();
                    previousButton.enabled = false;
                    nextButton.enabled = false

                }else if (folderModel.count >1 ) {
                    displayImage();
                    previousButton.enabled = false;
                    nextButton.enabled = true;
                }
                else{
                    previousButton.enabled = false;
                    nextButton.enabled = false;
                }
            }
        }
    }
}

