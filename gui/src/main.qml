import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0
import Qt.labs.folderlistmodel 2.12

import com.fwi 1.0

//window containing the application
ApplicationWindow {
//Window {

//    title of the application
    visible: true
    id: root
    title: qsTr("Full Waveform Inversion")
    width: 1200
    height: 900
    color: "#e6e6e5"

    Launcher {
          id: qLauncher
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


//    menu containing two menu items
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
//            MenuItem {
//                text: qsTr("&Open")
//                onTriggered: console.log("Open action triggered");
//            }
            MenuItem {
                text: qsTr("E&xit")
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

    ProgressBar {
        id: progressBar
        x: 64
        y: 840
        width: 1074
        height: 14
        value: 0
        anchors.bottomMargin: 10
    }

    Button {

        id: cancelButton
        x: 902
        y: 790
        text: qsTr("Cancel")
        onClicked: {
            Qt.quit()
        }
    }

    Button {
        id: computeButton
        x: 1038
        y: 790
        text: qsTr("Compute")
        transformOrigin: Item.TopLeft
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
                residualImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResidual.png"
                outputImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResult.png"
            }

            progressBar.value = 1
        }
    }

    function callExec(command) {
        return qLauncher.exec2("sh -c \"" + command + " \"");
    }

    ComboBox {
        id: forwardCombo
        x: 600
        y: 108
        width: 167
        height: 40
        flat: true
        currentIndex: 0
        Text {
            id: forwardText
            text: qsTr("Forward Model")
        }

        model: ["Integral", "FiniteDifference"]
    }

    ComboBox {
        id: inversionCombo
        x: 600
        y: 150
        width: 167
        height: 40
        flat: true
        Text {
            id: inversionText
            text: qsTr("Inversion Model")
        }
        currentIndex: 0
        model: ["ConjugateGradient", "GradientDescent", "Evolution", "Random"]
    }

    CheckBox {
        id: mpiCheckBox
        x: 843
        y: 108
        text: qsTr("MPI Parallel")
        opacity: 0.5
        ToolTip.visible: hovered
        ToolTip.text: qsTr("This feature is currently not enabled")
    }

    CheckBox {
        id: verboseCheckBox
        x: 843
        y: 150
        text: qsTr("Verbose")
        opacity: 0.5
        ToolTip.visible: hovered
        ToolTip.text: qsTr("This feature is currently not enabled")
    }

    Text {
        id: residualImageLabel
        x: 64
        y: 280
        text: qsTr("Residual graph")
        font.bold: true
        font.pixelSize: 16
    }

    Text {
        id: outputImageLabel
        x: 600
        y: 280
        text: qsTr("Output Images")
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
        x: 600
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

    Image {
        id: residualImage
        x: 64
        y: 300
        width: 450
        height: 320
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: outputImage
        x: 600
        y: 300
        width: 450
        height: 320
        fillMode: Image.PreserveAspectFit
    }

    ScrollView {
        id: scrollOutputText
        x: 64
        y: 650
        width: 1074
        height: 127
        clip: true
        Text {
            id: outputText

        }
    }

    Text {
        id: residualImageLabel1
        x: 64
        y: 630
        text: qsTr("Output")
        font.bold: true
        font.pixelSize: 16
    }

    Button {
        id: button
        x: 467
        y: 150
        width: 110
        height: 25
        text: qsTr("Select folder")
        onClicked: {
            fileDialog.open()
        }
    }

    Text {
        id: inputDataLabel1
        x: 64
        y: 190
        text: qsTr("Processing Steps")
        font.pixelSize: 16
        font.bold: true
    }

    CheckBox {
        id: preProcessingCheckBox
        x: 64
        y: 210
        text: qsTr("Pre-processing")
        checked: true
    }

    CheckBox {
        id: processingCheckBox
        x: 244
        y: 210
        text: qsTr("Processing")
        checked: true
    }

    CheckBox {
        id: postProcessingCheckBox
        x: 424
        y: 210
        text: qsTr("Post-processing")
        checked: true
    }
}
