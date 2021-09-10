import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0
import Qt.labs.folderlistmodel 2.12
import QtQuick.Controls.Styles 1.4

import com.fwi 1.0

ApplicationWindow {
    visible: true
    id: root
    title: qsTr("Full Waveform Inversion")
    width: 1200
    height: 500
    color: "#e6e6e5"

    Launcher {
          id: qLauncher
   }

    function callExec(command) {
        return qLauncher.exec2("sh -c \"" + command + " \"");
    }

    function selectOutputImage() {
        if (outputImageCombo.currentIndex == 0)
        {
            outputImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResult.png"
        }
        else if (outputImageCombo.currentIndex == 1)
        {
            outputImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultDummyImage.png"
        }
        else if (outputImageCombo.currentIndex == 2)
        {
            outputImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultChiDifference.png"
        }
    }

    menuBar: MenuBar {
        Menu {
                    title: qsTr("&File")
                    Action { text: qsTr("&Open...") }
                    Action { text: qsTr("&Save") }
                    Action { text: qsTr("Save &As...") }
                    MenuSeparator { }
                    Action {
                        text: qsTr("&Quit")
                        onTriggered: Qt.quit();
                    }
                }
        Menu {
            title: qsTr("&Help")
            MenuItem {
                text: qsTr("&About")
                onTriggered: {
                    var component = Qt.createComponent("extraWindow.qml");
                    var win = component.createObject(this);
                    win.show();
                }
            }
        }
    }

    ScrollView {
        width: 1200
        height: 600
        contentHeight: 1050  // Same
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

    Rectangle {
        width: 1180
        height: 880
        x: 10
        y: 10
        border.color: "black"
        border.width: 1
        radius: 5
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a folder"
        selectFolder: true
        folder: shortcuts.home
        onAccepted: {
            inputFolderTextEdit.text = fileDialog.folder.toString().replace("file://","")
            this.close()
        }
        onRejected: {
            this.close()
        }
    }

    ProgressBar {
        id: progressBar
        x: 64
        y: 280
        width: 965
        height: 14
        value: 0
        anchors.bottomMargin: 10
    }

    Button {
        id: cancelButton
        x: 920
        y: 230
        contentItem: Text {
            text: qsTr("Cancel")
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        background: Rectangle{
            implicitWidth: 100
            implicitHeight: 35
            border.width: 2
            border.color: "grey"
            radius: 4
            color: "light grey"
        }
        onClicked: {
            Qt.quit()
        }
    }

    Button {
        id: computeButton
        x: 750
        y: 230
        contentItem: Text {
            text: qsTr("Compute")
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        background: Rectangle{
            implicitWidth: 100
            implicitHeight: 35
            border.width: 2
            border.color: "grey"
            radius: 4
            color: "#440154" // darkest color in viridis colormap
        }
        onClicked: {
            progressBar.value = 0
            residualImage.source = ""
            outputImage.source = ""

            console.log(CurDirPath)
            console.log(inputFolderTextEdit.text, forwardCombo.currentText, inversionCombo.currentText)
            progressBar.value = 0.1

            if (preProcessingCheckBox.checked)
            {
                console.log("==== PREPROCESS")
                var preProcessCommand = inputFolderTextEdit.text+"/../bin/FWI_PreProcess -d '%DATA%' -f %FORWARD%"
                preProcessCommand = preProcessCommand.replace("%DATA%", inputFolderTextEdit.text)
                preProcessCommand = preProcessCommand.replace("%FORWARD%", forwardCombo.currentText)
                var output = callExec(preProcessCommand)
                outputText.text = output
            }
            progressBar.value = 0.33

            if (processingCheckBox.checked)
            {
                console.log("==== PROCESS")
                var processCommand = inputFolderTextEdit.text+"/../bin/FWI_Process -d '%DATA%' -f %FORWARD% -i %INVERSE%"
                processCommand = processCommand.replace("%DATA%", inputFolderTextEdit.text)
                processCommand = processCommand.replace("%FORWARD%", forwardCombo.currentText)
                processCommand = processCommand.replace("%INVERSE%", inversionCombo.currentText)
                output = callExec(processCommand)
                outputText.text = outputText.text + "\n" + output
            }
            progressBar.value = 0.66

            if (postProcessingCheckBox.checked)
            {
                console.log("==== POSTPROCESS")
                var postProcessCommand = "$(which python3) %BIN%postProcessing-python3.py -o '%DATA%'"
                postProcessCommand = postProcessCommand.replace("%BIN%", inputFolderTextEdit.text+"/../")
                postProcessCommand = postProcessCommand.replace("%DATA%", inputFolderTextEdit.text)
                console.log(postProcessCommand)
                output = callExec(postProcessCommand)
                outputText.text = outputText.text + "\n" + output
                selectOutputImage()
                residualImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResidual.png"
            }
            progressBar.value = 1
        }
    }

    ComboBox {
        id: forwardCombo
        x: 700
        y: 108
        width: 250
        height: 50
        flat: true
        currentIndex: 0
        Text {
            id: forwardText
            text: qsTr("Forward Model")
        }
        model: ["Integral", "FiniteDifference", "FiniteDifferenceParallel", "FiniteDifferenceParallelMPI"]
    }

    ComboBox {
        id: inversionCombo
        x: 700
        y: 150
        width: 250
        height: 50
        flat: true
        Text {
            id: inversionText
            text: qsTr("Inversion Model")
        }
        currentIndex: 0
        model: ["ConjugateGradient", "GradientDescent", "Evolution", "Random"]
    }

    Text {
        text: "Threads/Cores (Parallel Only)"
        font.family: "Helvetica"
        font.pixelSize: 16
        font.bold: true
        x: 950
        y: 82
    }

    SpinBox {
        value: 1
        from: 1
        to: 8
        stepSize: 1
        x: 1000
        y: 108

    }

    CheckBox {
        id: verboseCheckBox
        x: 1000
        y: 150
        text: qsTr("Verbose")
        opacity: 0.5
        ToolTip.visible: hovered
        ToolTip.text: qsTr("This feature is currently not enabled")
    }

    Text {
        id: residualImageLabel
        x: 540
        y: 300
        text: qsTr("Residual graph")
        font.bold: true
        font.pixelSize: 16
    }

    Text {
        id: outputImageLabel
        x: 64
        y: 300
        text: qsTr("Output Image")
        font.pixelSize: 16
        font.bold: true
    }

    Text {
        id: inputDataLabel
        x: 64
        y: 88
        text: qsTr("Input data")
        font.pixelSize: 16
        font.bold: true
    }

    Text {
        id: parametersLabel
        x: 690
        y: 88
        text: qsTr("Parameters")
        font.pixelSize: 16
        font.bold: true
    }

    TextEdit {
        id: inputFolderTextEdit
        x: 73
        y: 108
        width: 479
        height: 17
        font.pixelSize: 12

        text: qsTr("/home/jan/parallelized-fwi/FWIInstall/default")
        property string placeholderText: "Enter path/to/data."

        Text {
            text: inputFolderTextEdit.placeholderText
            color: "#aaa"
            visible: !inputFolderTextEdit.text && !inputFolderTextEdit.activeFocus
        }
    }

    Text {
        id: titleLabel
        x: 29
        y: 35
        text: qsTr("Full Waveform Inversion")
        font.pixelSize: 28
        font.bold: true
    }

    ComboBox {
        id: outputImageCombo
        x: 229
        y: 290
        width: 167
        height: 40
        currentIndex: 0
        model: ["Output values", "Dummy Image", "Residuals"]
        flat: true
        onCurrentIndexChanged:
        {
            if (outputImage.source != "")
            {
                selectOutputImage()
            }
        }
    }

    Image {
        id: outputImage
        x: 64
        y: 350
        width: 450
        height: 320
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: residualImage
        x: 540
        y: 350
        width: 450
        height: 320
        fillMode: Image.PreserveAspectFit
    }

    Rectangle{
        id: scrollViewParentRectangle
        x: 64
        y: 720
        width: 955
        height: 127
        border.width: 1
        border.color: "grey"
        radius: 3
        color: "#e6e6e5"
    }

    ScrollView {
        id: scrollOutputText
        anchors.centerIn: scrollViewParentRectangle
        width: scrollViewParentRectangle.width - 20
        height: scrollViewParentRectangle.height - 2
        clip: true

        Text {
            id: outputText
        }
    }

    Text {
        id: residualImageLabel1
        x: 64
        y: 700
        text: qsTr("")
        font.bold: true
        font.pixelSize: 16
    }

    Button {
        id: button
        x: 467
        y: 150
        width: 110
        height: 25
        contentItem: Text {
            text: qsTr("Select folder")
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        background: Rectangle{
            implicitWidth: 100
            implicitHeight: 35
            border.width: 2
            border.color: "grey"
            radius: 4
            color: "light grey"
        }

        onClicked: {
            fileDialog.open()
        }
    }

    Text {
        id: inputDataLabel1
        x: 64
        y: 210
        text: qsTr("Processing Steps")
        font.pixelSize: 16
        font.bold: true
    }

    CheckBox {
        id: preProcessingCheckBox
        x: 64
        y: 230
        text: qsTr("Pre-processing")
        checked: false
    }

    CheckBox {
        id: processingCheckBox
        x: 264
        y: 230
        text: qsTr("Processing")
        checked: false
    }

    CheckBox {
        id: postProcessingCheckBox
        x: 424
        y: 230
        text: qsTr("Post-processing")
        checked: false
    }


    }
}
