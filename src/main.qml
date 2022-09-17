import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import "controls" as Controls
import StableDiffusion 1.0
import Qt.labs.platform 1.1

ApplicationWindow {
    id: window

    width: 1200
    height: 900
    maximumHeight: 900
    minimumHeight: 900
    maximumWidth: 1200
    minimumWidth: 1200
    visible: true
    title: qsTr("SDImageGenerator")
    font.pointSize: 12
    property double startTime: 0

    TextToImageBackend{
        id : stableDiffusionBackend
    }

    MessageDialog
    {
        id: msgDialog

        title: qsTr("SDImageGenerator Error ")
        text: stableDiffusionBackend.errorMessage
        visible: false
        buttons: MessageDialog.Ok
       // icon: StandardIcon.Critical
    }

    Connections{
        target: stableDiffusionBackend

        function onShowMessageBox() {
            console.log("Show dialogue");
            msgDialog.visible = true;
        }

        function onInitControls(options) {
            promptInput.text = options.prompt;
            scaleSlider.slider.value = options.scale;
            widthSlider.slider.value = options.imageWidth;
            heightSlider.slider.value = options.imageHeight;
            imageCountSlider.slider.value = options.numberOfImages;
            ddimStepsSlider.slider.value = options.ddimSteps;
            samplerComboBox.currentIndex = samplerComboBox.indexOfValue(options.sampler);
            saveFolder.text = options.saveDir
            if (options.seed === 0)
                seedInput.text = "";
            else
                seedInput.text = options.seed;
        }

        function onSetOutputDirectory(directory){
            saveFolder.text = directory
        }
    }
    function getElapsedTime()
    {
        let elapsedSeconds = (new Date().getTime()-startTime)/1000.0
        let elapsedTimeFomatted =""
        if (elapsedSeconds>3600)
            elapsedTimeFomatted = new Date(elapsedSeconds * 1000).toISOString().slice(11, 19);
        else
            elapsedTimeFomatted = new Date(elapsedSeconds * 1000).toISOString().slice(14, 19);

        return elapsedTimeFomatted.toString();
    }

    function updateOptions(){

        stableDiffusionBackend.options.prompt = promptInput.text
        stableDiffusionBackend.options.scale = scaleSlider.slider.value.toFixed(1)
        stableDiffusionBackend.options.imageWidth = parseInt(widthSlider.slider.value)
        stableDiffusionBackend.options.imageHeight = parseInt(heightSlider.slider.value)
        stableDiffusionBackend.options.numberOfImages = parseInt(imageCountSlider.slider.value)
        stableDiffusionBackend.options.ddimSteps = parseInt(ddimStepsSlider.slider.value)
        stableDiffusionBackend.options.sampler = samplerComboBox.currentText
        stableDiffusionBackend.options.saveDir = saveFolder.text
        if (seedInput.text)
            stableDiffusionBackend.options.seed = seedInput.text
        else
            stableDiffusionBackend.options.seed = 0
    }

    Drawer {
        readonly property bool inPortrait: window.width < window.height
        id: drawer

        modal: inPortrait
        interactive: inPortrait
        position: inPortrait ? 0 : 1
        visible: !inPortrait
        width:480

        ScrollView
        {
            width: drawer.width
            height: window.height
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout
            {
                Item {
                    id: spacer
                    width: 25
                }

                ColumnLayout{
                    id : column
                    anchors.fill: drawer
                    spacing: 10

                    Controls.AppLabel{
                        Layout.topMargin: 30
                        labelText:qsTr("Sampler")
                        labelInfo: qsTr("The diffusion sampling method. Default is PLMS.")

                    }
                    Item{
                        height:45
                        ComboBox{
                            id: samplerComboBox
                            height:45
                            Layout.fillHeight: true

                            model: ["plms", "ddim"]
                        }
                    }

                    Controls.AppSlider{
                        id : scaleSlider

                        header.text: qsTr("Guidance scale")
                        description.text: qsTr("Higher values keep your image closer to your prompt.")
                        slider {
                            from:1
                            to:10
                            value: 7.5
                        }
                        Layout.fillWidth:true
                        displayFloat: true
                    }

                    Controls.AppSlider{
                        id: widthSlider

                        header.text: qsTr("Width")
                        description.text: qsTr("The width of the generated image.")
                        slider.from: 256
                        slider.to: 2000
                        slider.value: 512
                        Layout.fillWidth:true
                    }

                    Controls.AppSlider{
                        id: heightSlider

                        header.text: qsTr("Height")
                        description.text: qsTr("The height of the generated image.")
                        slider.from:256
                        slider.to:2000
                        slider.value: 512
                        Layout.fillWidth:true
                    }

                    Controls.AppSlider{
                        id: imageCountSlider

                        header.text: qsTr("Number of Images")
                        description.text: qsTr("Number of images to generate.")
                        slider.from: 1
                        slider.to: 20
                        slider.value: 1
                        Layout.fillWidth:true
                    }

                    Controls.AppSlider{
                        id: ddimStepsSlider

                        header.text: qsTr("DDIM Steps")
                        description.text: qsTr("Number of DDIM sampling steps")
                        slider.from:1
                        slider.to:150
                        slider.value: 50
                        Layout.fillWidth:true
                    }

                    Controls.AppLabel{
                        labelText:qsTr("Seed")
                        labelInfo: qsTr("Set a number to reproducible sampling.")
                    }

                    Controls.RichTextEdit{
                        id : seedInput

                        Layout.fillWidth:true
                        placeholderText: "Random seed"
                        validator: IntValidator {bottom: 1; top: 9999999}
                    }

                    Controls.AppLabel{
                        labelText:qsTr("Save path")
                        labelInfo: qsTr("Root folder to save generated images")

                    }
                    RowLayout{
                         Layout.fillWidth: true
                        Controls.RichTextEdit{
                          id: saveFolder
                          Layout.fillWidth: true

                        }
                        FolderDialog {
                            id: folderDialog
                            folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                             onAccepted: {
                                 stableDiffusionBackend.setOutputFolder(folder)
                             }
                        }
                        ToolButton{
                           // icon.color: "#26a8ff"
                            icon.source: "images/folder2-open.png"
                            onClicked: folderDialog.open()

                        }
                    }

                    Button{
                        text : "Reset All"
                        onClicked: stableDiffusionBackend.resetSettings()
                    }
                }
            }
        }
    }



    ColumnLayout{
        anchors.fill: parent
        anchors.leftMargin:  drawer.width

        Controls.ImageViewer{
            id: imageViewer

            Layout.alignment:Qt.AlignCenter
            currentImagePath: "../../images/placeholder.png"
            folderpath: stableDiffusionBackend.samplesUrl
        }

        Label{
            text:qsTr("Prompt")
            Layout.leftMargin: 20
        }
        RowLayout{
            Item{
                width:700
                height:100
                Layout.leftMargin: 20
                Layout.fillWidth: true
                ScrollView {
                    id: view
                    anchors.fill: parent
                    clip:true
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    Controls.AppTextArea{
                        id: promptInput
                        placeholderText: "A virus playing guitar"
                        font.pointSize: 12
                    }
                }
            }

            Button{
                //Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                Layout.rightMargin: 20
                text: "Dream"
                icon.color: "transparent"
                icon.source: "images/moon-stars-fill.png"
                enabled: !stableDiffusionBackend.isProcessing

                onClicked: {
                    updateOptions();
                    startTime = new Date().getTime();
                    stableDiffusionBackend.generateImage();
                }
            }
        }
        Button{
            id : buttonOpen
            text : "Open Output Folder"
            Layout.leftMargin: 20
            icon.source: "images/folder2-open.png"
            enabled: !stableDiffusionBackend.isProcessing
            onClicked: stableDiffusionBackend.openOutputFolder();
        }
        RowLayout{
            spacing: 10
            Item{
                Layout.leftMargin: 20
            }
            BusyIndicator {
                id: busyIndicator

                visible: running
                running: stableDiffusionBackend.isProcessing
            }

            Timer {
                   id: elasedTimer
                   interval: 1000;
                   running: stableDiffusionBackend.isProcessing;
                   repeat: true
                   onTriggered: {
                   //elapsedTimeLabel.
                    elapsedTimeLabel.text = getElapsedTime();
               }
            }
            Label {

                id:  elapsedTimeLabel
                text: ""
                font.pointSize: 10
                Layout.bottomMargin: 5
            }

            Label {
                text: stableDiffusionBackend.diffusionStatusMessage
                font.pointSize: 10
                Layout.bottomMargin: 5
            }
        }
    }
    onClosing: {
        updateOptions();
        stableDiffusionBackend.saveSettings();
    }

}
