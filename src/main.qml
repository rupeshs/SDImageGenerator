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

    width: 600
    height: 600
    maximumHeight: 600
    minimumHeight: 600
    maximumWidth: 600
    minimumWidth: 600
    visible: true
    title: qsTr("SDImageGenerator")
    font.pointSize: 12
    property double startTime: 0
    property bool initApp: true;


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

        function onInitControls(options,envStatus) {
            promptInput.text = options.prompt;
            scaleSlider.slider.value = options.scale;
            widthSlider.slider.value = options.imageWidth;
            heightSlider.slider.value = options.imageHeight;
            imageCountSlider.slider.value = options.numberOfImages;
            ddimStepsSlider.slider.value = options.ddimSteps;
            samplerComboBox.currentIndex = samplerComboBox.indexOfValue(options.sampler);
            saveFolder.text = options.saveDir
            seedInput.text = options.seed;
            gridCheckBox.checked = options.grid;
            seamlessCheckBox.checked = options.seamless;
            fullPrcisionCheckBox.checked = options.fullPrecision;

            upscalerCheckBox.checked = options.upscaler;
            upscaleStrengthSlider.slider.value = options.upscaleStrength;
            if (options.upscaleFactor === "4x")
                radiobuttonScaleFour.checked = true
            else
                radiobuttonScaleTwo.checked = true

            modelCheck.checked = envStatus.isStableDiffusionModelReady;
            modelCheck.checkable = false;
            gfpganCheckBox.checked = options.faceRestoration;
            gfpganStrengthSlider.slider.value = options.faceRestorationStrength;
            saveOriginalCheckBox.checked = options.saveOriginalImage;
            gfpganModelCheck.checked = envStatus.isGfpGanModelReady;

            imgtoimgCheckBox.checked = options.imageToImage;
            imgStrength.slider.value = options.imageToImageStrength;
            fitImageCheckBox.checked = options.fitImage;
            initImagePathTextEdit.text = options.initImagePath;

        }

        function onSetOutputDirectory(directory){
            saveFolder.text = directory;
        }
        function onSetInputImagePath(file_path){
            initImagePathTextEdit.text = file_path;
        }
        function onIsProcessingChanged() {
            if (!stableDiffusionBackend.isProcessing)
            {
                tabBar.currentIndex = 1;
            }
        }
        function onEnvironmentNotReady() {
            tabBar.currentIndex = 4;
            tabBar.itemAt(0).enabled = false;
            tabBar.itemAt(1).enabled = false;
            tabBar.itemAt(2).enabled = false;
            tabBar.itemAt(3).enabled = false;
        }

        function onCloseLoadingScreen() {
            downloadDialog.visible = false;
        }

        function onSetupInstallerUi(isDownloader) {

            downloadDialog.isDownloader = isDownloader;
            if (isDownloader) {
                installerDownloadPbar.visible = true;
                installerStatusLabel.font.pointSize = 9;
                downloadDialog.title = qsTr("SDImageGenerator - Downloading model...");
                downloadDialog.visible = true ;
            } else {
                installerDownloadPbar.visible = false;
                installerStatusLabel.font.pointSize = 12;
                downloadDialog.title = qsTr("SDImageGenerator - Loading...");
                downloadDialog.visible = true ;
            }

        }

        function onInstallerStatusChanged(msg , progress) {
            installerStatusLabel.text = msg;
        }
        function onDownloaderStatusChanged(msg , progress) {
            installerStatusLabel.text = msg;
            installerDownloadPbar.value = progress.toFixed(2);
            busyIndicatorInstaller.visible = false;
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

        stableDiffusionBackend.options.prompt = promptInput.text;
        stableDiffusionBackend.options.scale = scaleSlider.slider.value.toFixed(1);
        stableDiffusionBackend.options.imageWidth = parseInt(widthSlider.slider.value);
        stableDiffusionBackend.options.imageHeight = parseInt(heightSlider.slider.value);
        stableDiffusionBackend.options.numberOfImages = parseInt(imageCountSlider.slider.value);
        stableDiffusionBackend.options.ddimSteps = parseInt(ddimStepsSlider.slider.value);
        stableDiffusionBackend.options.sampler = samplerComboBox.currentText;
        stableDiffusionBackend.options.saveDir = saveFolder.text;
        stableDiffusionBackend.options.seed = seedInput.text;
        stableDiffusionBackend.options.grid = gridCheckBox.checked;
        stableDiffusionBackend.options.seamless = seamlessCheckBox.checked;
        stableDiffusionBackend.options.fullPrecision = fullPrcisionCheckBox.checked;
        stableDiffusionBackend.options.upscaler = upscalerCheckBox.checked ;
        stableDiffusionBackend.options.upscaleStrength = upscaleStrengthSlider.slider.value.toFixed(2);
        stableDiffusionBackend.options.upscaleFactor = upscaleFactorGroup.checkedButton.text;
        stableDiffusionBackend.options.faceRestoration = gfpganCheckBox.checked;
        stableDiffusionBackend.options.faceRestorationStrength = gfpganStrengthSlider.slider.value.toFixed(2);
        stableDiffusionBackend.options.saveOriginalImage = saveOriginalCheckBox.checked;
        stableDiffusionBackend.options.imageToImage= imgtoimgCheckBox.checked;
        stableDiffusionBackend.options.imageToImageStrength = imgStrength.slider.value.toFixed(2);
        stableDiffusionBackend.options.fitImage = fitImageCheckBox.checked;
        stableDiffusionBackend.options.initImagePath = initImagePathTextEdit.text;
    }
    TabBar {
        id: tabBar
        width: parent.width

        TabButton {
            //text: qsTr("Text to Image")
            icon.source: "images/moon-stars-fill.png"
            icon.width: 28
            icon.height: 28
            background: Rectangle {
                //#26a8ff
                color: tabBar.currentIndex == 0 ? "green": "#393A3B"
            }

        }
        TabButton {
           // text: qsTr("Images")
            icon.source: "images/images.png"
            icon.width: 28
            icon.height: 28
            background: Rectangle {
                color: tabBar.currentIndex == 1 ? "green": "#393A3B"
            }

        }
        TabButton {
            //text: qsTr("Settings")
            icon.source: "images/gear.png"
            icon.width: 28
            icon.height: 28

            background: Rectangle {
                color: tabBar.currentIndex == 2 ? "green": "#393A3B"
            }
        }
        TabButton {
            //text: qsTr("Enhance")
            icon.source: "images/enhance.png"
            icon.width: 28
            icon.height: 28
            background: Rectangle {
                color: tabBar.currentIndex == 3 ? "green": "#393A3B"
            }

        }
        TabButton {
            //text: qsTr("Install")
            icon.source: "images/download.png"
            icon.width: 28
            icon.height: 28
            background: Rectangle {
                color: tabBar.currentIndex == 4 ? "green": "#393A3B"
            }
        }


        TabButton {
            //text: qsTr("About")
            icon.source: "images/info-square.png"
            icon.width: 28
            icon.height: 28
            background: Rectangle {
                color: tabBar.currentIndex == 5 ? "green": "#393A3B"
            }

        }
        onCurrentIndexChanged: {
            layout.currentIndex = tabBar.currentIndex
        }

    }
    StackLayout {
        id: layout
        anchors.fill: parent
        currentIndex: 0
        anchors.topMargin: tabBar.height
        Page{
            RowLayout{
                Item{
                    width:20
                }
                ColumnLayout{
                    Item{
                        height: 70
                    }
                    Label{
                        text:qsTr("Prompt")
                    }

                    Item{
                        width: 550
                        height:100
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter


                        ScrollView {
                            id: view
                            anchors.fill: parent
                            clip:true
                            ScrollBar.vertical.policy: ScrollBar.AsNeeded

                            Controls.AppTextArea{
                                id: promptInput
                                placeholderText: "A fantasy landscape"
                                font.pointSize: 12
                            }
                        }
                    }

                    RowLayout{

                        Button{
                            //Layout.fillHeight: true
                            Layout.alignment: Qt.AlignRight
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
                        Button{
                            Layout.alignment: Qt.AlignRight
                            text: "Logs"
                            icon.color: "white"
                            icon.source: "images/file-text.png"


                            onClicked: {

                                stableDiffusionBackend.openLogs();
                            }
                        }

                        Item{
                            Layout.fillWidth: true  
                        }
                        BusyIndicator {
                            id: busyIndicator
                            visible: running
                            Layout.rightMargin: 10
                            running: stableDiffusionBackend.isProcessing
                        }

                        Label {
                            id:  elapsedTimeLabel

                            Layout.alignment: Qt.AlignRight
                            text: "00:00"
                            color: "gray"
                            font.pointSize: 10
                            Layout.bottomMargin: 5
                        }
                    }
                    Item{
                    height: 20
                    }
                    GroupBox{
                        id: imgimgGroup
                        Layout.preferredWidth: 550
                        Layout.leftMargin: 10

                        label: CheckBox {
                            id: imgtoimgCheckBox
                            checked: false
                            text: qsTr("Image to Image")


                        }
                        ColumnLayout{
                          enabled: imgtoimgCheckBox.checked
                          anchors.fill: parent

                          FileDialog {
                              id: imgFileDialog
                              title: "Please choose a image"

                              nameFilters: [ "Image files (*.jpg *.png *.jpeg)", "All files (*)" ]
                              onAccepted: {
                                  console.log("You chose: ",imgFileDialog.file)
                                  stableDiffusionBackend.setImageInput(imgFileDialog.file);
                                  imgFileDialog.close();

                              }

                          }
                          RowLayout{
                              Layout.fillWidth: true
                              Controls.RichTextEdit{
                                  id: initImagePathTextEdit
                                  Layout.fillWidth: true
                              }
                              ToolButton{
                                  icon.source: "images/folder2-open.png"
                                  onClicked: imgFileDialog.open()
                              }
                          }

                       }
                    }

                }
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
            footer:RowLayout{
                Item{
                    width : 5
                }
                Label {
                    text: stableDiffusionBackend.diffusionStatusMessage
                    font.pointSize: 10
                    Layout.bottomMargin: 15
                }
            }
        }

        Page{
            id: paneImages

            Controls.FolderImageViewer{
                folderPath: stableDiffusionBackend.samplesUrl
            }


            Button {
                width: 48
                height: 48
                Layout.alignment:  Qt.AlignBottom
                icon.source: "images/folder2-open.png"
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Open output folder")
                onClicked: stableDiffusionBackend.openOutputFolder();
            }
        }

        ScrollView
        {
            //width: window.width
            //height: window.height
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            RowLayout
            {
                width: 550
                Item {
                    id: spacer
                    width: 25
                }

                ColumnLayout{
                    id : column
                    spacing: 10

                    Item{
                        height:30
                    }

                    Controls.AppLabel{
                        labelText:qsTr("Sampler")
                        labelInfo: qsTr("The diffusion sampling method. Default is K_LMS.")

                    }
                    Item{
                        height:45
                        ComboBox{
                            id: samplerComboBox
                            height:45
                            width: 150
                            Layout.fillHeight: true
                            model: ['ddim',
                                'k_dpm_2_a',
                                'k_dpm_2',
                                'k_euler_a',
                                'k_euler',
                                'k_heun',
                                'k_lms',
                                'plms']
                            enabled: !imgtoimgCheckBox.checked

                        }
                    }

                    Controls.AppSlider{
                        id : scaleSlider

                        header.text: qsTr("Guidance scale")
                        description.text: qsTr("Higher values keep your image closer to your prompt.")
                        slider {

                            from: 1.1
                            to: 20
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
                        slider.to: 2048
                        slider.value: 512
                        slider.stepSize: 64
                        Layout.fillWidth:true

                    }

                    Controls.AppSlider{
                        id: heightSlider

                        header.text: qsTr("Height")
                        description.text: qsTr("The height of the generated image.")
                        slider.from:256
                        slider.to:2048
                        slider.value: 512
                        slider.stepSize: 64
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
                        validator: RegExpValidator { regExp: /[0-9]+/ }
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
                            icon.source: "images/folder2-open.png"
                            onClicked: folderDialog.open()

                        }
                    }

                    CheckBox{
                      id : gridCheckBox

                      text: "Grid"
                    }
                    Controls.AppInfoLabel{
                        textInfo: qsTr("Turn on grid mode to return a single image combining all the images")
                    }

                    CheckBox{
                      id : seamlessCheckBox

                      text: "Seamless tiling"
                    }

                    Controls.AppInfoLabel{
                        textInfo: qsTr("Activate seamless tiling for interesting effects")
                    }
                    CheckBox{
                      id : fullPrcisionCheckBox

                      text: "Full precision"
                      onCheckedChanged: {
                          if (!initApp){
                              msgDialog.title = "SDImageGenerator";
                              msgDialog.text = "Please restart the app to apply the setting.";
                              msgDialog.visible = true;
                          }
                          initApp = false;
                      }
                    }
                    Controls.AppInfoLabel{
                        textInfo: qsTr("Run in slower full-precision mode.Needed for some older video cards.")
                    }

                    CheckBox{
                        id: fitImageCheckBox
                        text: qsTr("Fit (Image to Image)")
                        checked:false
                    }
                    Controls.AppInfoLabel{
                        textInfo: qsTr("Scale the image to fit into the specified width and height")
                    }

                   Controls.AppSlider{
                        id: imgStrength

                        header.text: qsTr("Image to Image Strength")
                        description.text: qsTr("Controls how much the original will be modified")
                        slider.from: 0
                        slider.to: 0.99
                        slider.value: 0.75
                        displayFloat: true
                        decimalPointNumbers: 2
                        Layout.fillWidth:true

                    }

                    Button{
                        text : "Reset All"
                        onClicked: stableDiffusionBackend.resetSettings()
                    }
                }
            }
        }
        Page{
            id : pageEnhance
            Layout.alignment: Qt.AlignCenter
            ColumnLayout{
                Item{
                    height: 10
                }
                CheckBox{
                    id :saveOriginalCheckBox
                    text: "Save original image"

                    Layout.leftMargin: 10
                    enabled: upscalerCheckBox.checked | gfpganCheckBox.checked
                }

                GroupBox{
                    id: upscalerGroup
                    Layout.preferredWidth: window.width - 20
                    Layout.leftMargin: 10

                    label: CheckBox {
                        id: upscalerCheckBox
                        checked: false
                        text: qsTr("Upscaler (Real-ESRGAN)")
                    }
                    ColumnLayout{
                        anchors.fill: parent
                        enabled: upscalerCheckBox.checked


                        ButtonGroup {
                            id: upscaleFactorGroup

                            buttons: rowScaleFactor.children
                        }
                        RowLayout{
                            id : rowScaleFactor
                            Label{
                                text : "Scaling factor :";
                            }
                            RadioButton {
                                id : radiobuttonScaleTwo

                                checked: true
                                text: qsTr("2x")
                            }
                            RadioButton {
                                id : radiobuttonScaleFour

                                text: qsTr("4x")
                            }
                        }
                        Controls.AppSlider{
                            id: upscaleStrengthSlider

                            header.text: qsTr("Upscaling strength")
                            description.text : "For natural looking results, we recommend using values between 0.5 to 0.8"
                            slider.from: 0
                            slider.to: 1
                            slider.value: 0.75
                            Layout.fillWidth: true
                            displayFloat: true
                            decimalPointNumbers : 2
                        }
                    }
                }
                GroupBox{
                    id: gfpganGroup
                    Layout.preferredWidth: window.width - 20
                    Layout.leftMargin: 10

                    label: CheckBox {
                        id: gfpganCheckBox
                        checked: false
                        text: qsTr("Face Restoration (GFPGAN)")
                    }
                    ColumnLayout{
                    anchors.fill: parent
                    enabled: gfpganCheckBox.checked

                    Controls.AppSlider{
                        id: gfpganStrengthSlider

                        header.text: qsTr("Face restoration strength")
                        description.text : "Controls the strength of the face restoration,we recommend using values between 0.5 to 0.8"
                        slider.from: 0
                        slider.to: 1
                        slider.value: 0.75
                        Layout.fillWidth: true
                        displayFloat: true
                        decimalPointNumbers : 2
                    }
                   }
                }
            }
        }

        Page {

            ColumnLayout{

                Label{
                    text : "Available models"
                    Layout.fillWidth: true
                    Layout.leftMargin: 20
                    Layout.topMargin: 30
                }

                Item{
                    Layout.fillWidth: true
                    height: 20
                }
                RowLayout{
                    CheckBox {
                        id : modelCheck

                        checkable: false
                        Layout.leftMargin: 10
                        text: qsTr("Stable diffusion model v1.4 original")
                    }
                    Button{
                        text : "Download model"
                        icon.source:  "images/file-arrow-down.png"
                        onClicked:  {
                            stableDiffusionBackend.downloadModel();
                        }

                    }
                }
                RowLayout{
                    CheckBox {
                        id : gfpganModelCheck

                        checkable: false
                        Layout.leftMargin: 10
                        text: qsTr("GFPGAN model (Optional) ")
                    }
                    Button{
                        text : "Download model"
                        icon.source:  "images/file-arrow-down.png"
                        onClicked:  {
                            stableDiffusionBackend.downloadGfpganModel();
                        }

                    }
                }

            }
        }

        Page {
            ColumnLayout{
                anchors.centerIn: parent
                Text{
                    text : qsTr("SDImageGenerator v0.1.5 Beta")
                    color : "white"
                    font.pointSize: 14
                    Layout.alignment: Qt.AlignHCenter

                }
                Text{
                    text : qsTr("Text to image generator AI")
                    color : "grey"
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: 12

                }
                Text{
                    text : qsTr("SDImageGenerator is a stable diffusion frontend")
                    color : "grey"
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter

                }

                Text{
                    text : qsTr("Copyright © 2022 Rupesh Sreeraman")
                    color : "grey"
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: "Using stable diffusion model: <a href='https://github.com/rupeshs/stablediffusion-mod/blob/main/Stable_Diffusion_v1_Model_Card.md'>Model card</a>"
                    color : "grey"
                    linkColor: "lightblue"
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                    onLinkActivated: Qt.openUrlExternally(link)
                }
                Text {
                    text: "<a href='https://github.com/rupeshs/SDImageGenerator/blob/main/Terms%20of%20use.md'>Terms of use</a>"
                    color : "grey"
                    linkColor: "lightblue"
                    font.pointSize: 12
                    Layout.alignment: Qt.AlignHCenter
                    onLinkActivated: Qt.openUrlExternally(link)
                }

            }


        }
    }

    onClosing: {
        window.title = qsTr("SDImageGenerator - Stopping please wait...");
        updateOptions();
        stableDiffusionBackend.saveSettings();
        stableDiffusionBackend.stopProcessing();

    }

    ApplicationWindow  {
        id: downloadDialog

        property bool isDownloader

        width: 550
        height: 200
        color: "#2e2e2e"
        title: "Installing..."
        maximumHeight:  200
        minimumHeight:  200
        maximumWidth:  550
        minimumWidth:  550
        flags: Qt.Dialog

        ColumnLayout{
            anchors.centerIn: parent
            spacing:  10

            Controls.CircularProgressBar{
                id: installerDownloadPbar

                Layout.alignment: Qt.AlignHCenter
                lineWidth: 10
                value: 0.0
                size: 150
                secondaryColor: "#e0e0e0"
                primaryColor: "#29b6f6"

                Text {
                    text: parseInt(installerDownloadPbar.value * 100) + "%"
                    anchors.centerIn: parent
                    font.pointSize: 20
                    color: installerDownloadPbar.primaryColor
                }
            }
            RowLayout{
            spacing : 10
            BusyIndicator{
              id : busyIndicatorInstaller
            }

            Label{
                id: installerStatusLabel
                text: "Please wait..."
                Layout.alignment: Qt.AlignBottom
                color: "white"
            }
            }
        }
        onClosing : {
            downloadDialog.title = qsTr("Stopping please wait...")
            if (isDownloader)
                stableDiffusionBackend.stopDownloader();
            else
                stableDiffusionBackend.stopInstaller();
        }
    }
}
