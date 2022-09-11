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
            console.log("Show dialogue");
            msgDialog.visible = true;
        }
    }


    Drawer {
        id: drawer
        width: 515
        height: window.height

        Pane{
            id: frame
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
                        labelText:qsTr("Sampler")
                        labelInfo: qsTr("The diffusion sampling method. Default is PLMS.")
                    }
                    ComboBox{
                        id: samplerComboBox
                        model: ["plms", "ddim"]
                    }

                    Controls.AppLabel{
                        labelText:qsTr("Guidance scale")
                        labelInfo: qsTr("Higher values keep your image closer to your prompt.")
                    }
                    Controls.AppSlider{
                        id : cfgScale
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
                        id: widthSlider
                        from:256
                        to:2000
                        value: 512
                        Layout.fillWidth:true
                        onValueChanged:TextToImageBackend.imageWidth = value

                    }
                    Controls.AppLabel{
                        labelText:qsTr("Height")
                        labelInfo: qsTr("The height of the generated image.")
                    }
                    Controls.AppSlider{
                        id: heightSlider
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
                        id: imageCountSlider
                        from:1
                        to:10
                        value: 1
                        Layout.fillWidth:true

                    }

                    Controls.AppLabel{
                        labelText:qsTr("DDIM Steps")
                        labelInfo: qsTr("Number of images to generate.")
                    }

                    Controls.AppSlider{
                        id: ddimStepsSlider
                        from:1
                        to:150
                        value: 50
                        Layout.fillWidth:true

                    }
                }
            }}
    }

    ColumnLayout{
        anchors.fill: parent
        Controls.ImageViewer{
            id: imageViewer
            Layout.alignment:Qt.AlignCenter
            currentImagePath: "../../images/placeholder-image.png"
            folderpath: stableDiffusionBackend.outputPath
        }
        Label{

            text:qsTr("Prompt")
            Layout.leftMargin: 10

        }
        RowLayout{
            Controls.RichTextEdit{
                id: promptInput
                Layout.leftMargin: 10
                placeholderText: "A virus playing guitar"
                font.pointSize: 10
                Layout.fillWidth: true


            }
            Button{
                text: "Generate"
                onClicked: {
                    stableDiffusionBackend.options.prompt = promptInput.text
                    stableDiffusionBackend.options.scale = cfgScale.value.toFixed(1)
                    stableDiffusionBackend.options.imageWidth = parseInt(widthSlider.value)
                    stableDiffusionBackend.options.imageHeight = parseInt(heightSlider.value)
                    stableDiffusionBackend.options.numberOfImages = parseInt(imageCountSlider.value)
                    stableDiffusionBackend.options.ddimSteps = parseInt(ddimStepsSlider.value)
                    stableDiffusionBackend.options.sampler = samplerComboBox.currentValue
                    stableDiffusionBackend.generateImage();

                }
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
    footer:
        Label {
        text: stableDiffusionBackend.diffusionStatusMessage
        font.pointSize: 10
        padding: 15

    }
}
