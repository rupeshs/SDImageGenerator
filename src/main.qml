import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import "controls" as Controls
import StableDiffusion 1.0
import QtQuick.Dialogs 1.3


ApplicationWindow {
    id: window
    width: 1400
    height: 800
    maximumHeight: 800
    minimumHeight: 800
    maximumWidth:  1400
    minimumWidth:  1400
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

        function onInitControls(options) {
            promptInput.text = options.prompt;
            scaleSlider.value = options.scale;
            widthSlider.value = options.imageWidth;
            heightSlider.value = options.imageHeight;
            imageCountSlider.value = options.numberOfImages;
            ddimStepsSlider.value = options.ddimSteps;
            samplerComboBox.currentIndex = samplerComboBox.indexOfValue(options.sampler);
            if (options.seed === 0)
                seedInput.text = "";
            else
                seedInput.text = options.seed;
        }
    }
    function updateOptions(){

        stableDiffusionBackend.options.prompt = promptInput.text
        stableDiffusionBackend.options.scale = scaleSlider.value.toFixed(1)
        stableDiffusionBackend.options.imageWidth = parseInt(widthSlider.value)
        stableDiffusionBackend.options.imageHeight = parseInt(heightSlider.value)
        stableDiffusionBackend.options.numberOfImages = parseInt(imageCountSlider.value)
        stableDiffusionBackend.options.ddimSteps = parseInt(ddimStepsSlider.value)
        stableDiffusionBackend.options.sampler = samplerComboBox.currentValue
        if (seedInput.text)
            stableDiffusionBackend.options.seed = seedInput.text
        else
            stableDiffusionBackend.options.seed = 0
    }

    Drawer {
        readonly property bool inPortrait: window.width < window.height
        id: drawer
        width: 515
        height: window.height
        modal: inPortrait
               interactive: inPortrait
               position: inPortrait ? 0 : 1
               visible: !inPortrait

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
                        id : scaleSlider
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
                        from: 1
                        to: 20
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
                    Controls.AppLabel{
                        labelText:qsTr("Seed")
                        labelInfo: qsTr("Set a seed to reproducible sampling.")

                    }
                    Controls.RichTextEdit{
                        id : seedInput
                        placeholderText: "Random seed"
                        validator: IntValidator {bottom: 1; top: 9999999}

                    }
                    Button{
                        text : "Reset All"
                        onClicked: stableDiffusionBackend.resetSettings()
                    }

                }
            }}
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.leftMargin:  drawer.width
        Controls.ImageViewer{
            id: imageViewer

            Layout.alignment:Qt.AlignCenter
            currentImagePath: "../../images/placeholder.png"
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
                text: "Dream"
                icon.color: "transparent"
                icon.source: "images/moon-stars-fill.png"

                 Layout.rightMargin: 10
                onClicked: {
                    updateOptions();
                    stableDiffusionBackend.generateImage();

                }
            }

        }
        Label {

        text: stableDiffusionBackend.diffusionStatusMessage
        font.pointSize: 10
        padding: 15

      }
    }
    onClosing: {
            updateOptions();
            stableDiffusionBackend.saveSettings();
        }


}
