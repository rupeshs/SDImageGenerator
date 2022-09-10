import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import "controls" as Controls
import StableDiffusion 1.0
import QtQuick.Dialogs 1.3


ApplicationWindow {
    id: window
    width: 900
    height: 800
    maximumHeight: 800
    minimumHeight: 800
    maximumWidth:  900
    minimumWidth:  900
    visible: true
    title: qsTr("SDImageGenerator")
    font.pointSize: 12

    TextToImageBackend{
        id : stableDiffusionBackend
    }

    MessageDialog
    {
        id: msgDialog
        title: qsTr("SDImageGenerator Error ")
        text: stableDiffusionBackend.errorMessage
        visible: false
        icon: StandardIcon.Critical
    }

    Connections{
        target: stableDiffusionBackend
        function onShowMessageBox() {
            msgDialog.visible = true;
        }
    }


    Drawer {
        id: drawer
        width: 515
        height: window.height

        Pane{
            id: frame
            z:1
            Layout.fillWidth: true
            ScrollView
            {
                width: 500
                height: window.height
                clip:true
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                ColumnLayout{
                    id : column
                    spacing: 10

                    Label{
                        text:qsTr("Settings")
                        font.bold: true
                        Layout.bottomMargin: 20
                        Layout.topMargin: 20
                        Layout.alignment:Qt.AlignHCenter

                    }

                    Controls.AppLabel{
                        labelText:qsTr("Guidance scale")
                        labelInfo: qsTr("Higher values keep your image closer to your prompt.")
                    }
                    Controls.AppSlider{
                        from:1
                        to:10
                        value: 7.5
                        Layout.fillWidth:true
                        displayFloat: true
                    }

                    Controls.AppLabel{
                        labelText:qsTr("Width")
                        labelInfo: qsTr("The width of the generated image.")
                    }
                    Controls.AppSlider{
                        from:256
                        to:2000
                        value: 512
                        Layout.fillWidth:true

                    }
                    Controls.AppLabel{
                        labelText:qsTr("Height")
                        labelInfo: qsTr("The height of the generated image.")
                    }
                    Controls.AppSlider{
                        from:256
                        to:2000
                        value: 512
                        Layout.fillWidth:true

                    }
                    Controls.AppLabel{
                        labelText:qsTr("Number of Images")
                        labelInfo: qsTr("Number of images to generate.")
                    }

                    Controls.AppSlider{
                        from:1
                        to:10
                        value: 1
                        Layout.fillWidth:true

                    }

                    Button{
                        text:"Start"
                        onClicked: stableDiffusionBackend.generateImage()

                    }
                    Button{
                        text:"Stop"
                        onClicked: stableDiffusionBackend.stopProcessing()

                    }
                    Slider {

                    }
                    Slider {

                    }
                    Slider {

                    }
                    Slider {

                    }
                    Slider {

                    }
                    Slider {

                    }
                    Slider {

                    }
                    Slider {

                    }

                }
            }}
    }





    ColumnLayout{
        anchors.fill: parent
        Controls.ImageViewer{
            id: imageViewer

            //anchors.centerIn: parent
            Layout.alignment:Qt.AlignCenter

            currentImagePath: "../../images/placeholder-image.png"
            //placeHolderImageSource: ":images/placeholder-image.png"
            //folderpath: "file:\d:\\textimg\\apps\\sdinstaller\\stable-diffusion\\outputs\\txt2img-samples\\Cyberpunk_style_image_of_a_Telsa_car_reflection_in_rain"
            folderpath: stableDiffusionBackend.outputPath
        }
        Label{
            text:qsTr("Prompt")
            Layout.leftMargin: 10

        }
        RowLayout{
            Controls.RichTextEdit{
                Layout.leftMargin: 10
                placeholderText: "A virus playing guitar"
                font.pointSize: 10
                Layout.fillWidth: true

            }
            Button{
                text: "Generate"
            }
            ToolButton{
                icon.color: "transparent"
                icon.source:  "images/icons8-settings-48.png"
                Layout.rightMargin: 10
                onClicked: drawer.visible=true
                icon.width:32
                icon.height:32

            }
        }

    }



    //Layout.alignment: Qt.AlignTop

    /* Controls.ImageViewer{
                 id: imageViewer
                Layout.alignment: Qt.AlignHCenter |Qt.AlignVCenter
                currentImagePath: "../../images/placeholder-image.png"
                //placeHolderImageSource: ":images/placeholder-image.png"
                //folderpath: "file:\d:\\textimg\\apps\\sdinstaller\\stable-diffusion\\outputs\\txt2img-samples\\Cyberpunk_style_image_of_a_Telsa_car_reflection_in_rain"
                folderpath: stableDiffusionBackend.outputPath
            }*/











    // anchors.fill: parent
    /* Pane{
            id: frame
            z:1
            Layout.fillWidth: true
            ScrollView
            {
                width: 500
                height: window.height
                clip:true
                 ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        ColumnLayout{
            id : column
            spacing: 10

           Label{
                text:qsTr("Prompt")
            }
           Controls.RichTextEdit{
              Layout.fillWidth:true
              placeholderText: "A virus playing guitar"
              font.pointSize: 14

           }
           Controls.AppLabel{
                labelText:qsTr("Guidance scale")
                labelInfo: qsTr("Higher values keep your image closer to your prompt.")
            }
           Controls.AppSlider{
               from:1
               to:10
               value: 7.5
               Layout.fillWidth:true
               displayFloat: true
           }

           Controls.AppLabel{
                labelText:qsTr("Width")
                labelInfo: qsTr("The width of the generated image.")
            }
           Controls.AppSlider{
               from:256
               to:2000
               value: 512
               Layout.fillWidth:true

           }
           Controls.AppLabel{
                labelText:qsTr("Height")
                labelInfo: qsTr("The height of the generated image.")
            }
           Controls.AppSlider{
               from:256
               to:2000
               value: 512
               Layout.fillWidth:true

           }
           Controls.AppLabel{
                labelText:qsTr("Number of Images")
                labelInfo: qsTr("Number of images to generate.")
            }

           Controls.AppSlider{
               from:1
               to:10
               value: 1
               Layout.fillWidth:true

           }

           Button{
               text:"Start"
               onClicked: stableDiffusionBackend.generateImage()

           }
           Button{
               text:"Stop"
               onClicked: stableDiffusionBackend.stopProcessing()

           }
           Slider {

           }
           Slider {

           }
           Slider {

           }
           Slider {

           }
           Slider {

           }
           Slider {

           }
           Slider {

           }
           Slider {

           }

        }
        }}*/





    footer:
        Label {
        text: stableDiffusionBackend.diffusionStatusMessage
        font.pointSize: 12
        color: "darkgreen"
        padding: 15
        z:4


    }
}
