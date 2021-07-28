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
    height: 800
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
        y: 786
        width: 1074
        height: 14
        value: 0
    }

    Button {Model size of -46 is less than 0

        id: cancelButton
        x: 902
        y: 740
        text: qsTr("Cancel")
        onClicked: {
            Qt.quit()
        }
    }

    Button {
        id: computeButton
        x: 1038
        y: 740
        text: qsTr("Compute")
        transformOrigin: Item.TopLeft
        onClicked: {
            console.log(CurDirPath)
            console.log(inputFolderTextEdit.text, forwardCombo.currentText, inversionCombo.currentText)
            progressBar.value = 0.1

            console.log("==== PREPROCESS")
            var preProcessCommand = CurDirPath+"../../../FWIInstall/bin/FWI_PreProcess -d '%DATA%' -f %FORWARD%"
            preProcessCommand = preProcessCommand.replace("%DATA%", inputFolderTextEdit.text)
            preProcessCommand = preProcessCommand.replace("%FORWARD%", forwardCombo.currentText)
            var output = callExec(preProcessCommand)
            outputText.text = output
            progressBar.value = 0.33

            console.log("==== PROCESS")
            var processCommand = CurDirPath+"../../../FWIInstall/bin/FWI_Process -d '%DATA%' -f %FORWARD% -i %INVERSE%"
            processCommand = processCommand.replace("%DATA%", inputFolderTextEdit.text)
            processCommand = processCommand.replace("%FORWARD%", forwardCombo.currentText)
            processCommand = processCommand.replace("%INVERSE%", inversionCombo.currentText)
            output = callExec(processCommand)
            outputText.text = outputText.text + "\n" + output
            progressBar.value = 0.66

            console.log("==== POST PROCESS")
            var postProcessCommand = "$(which python3) %BIN%postProcessing-python3.py -o '%DATA%'"
            postProcessCommand = postProcessCommand.replace("%BIN%", CurDirPath+"../../../FWIInstall/")
            postProcessCommand = postProcessCommand.replace("%DATA%", CurDirPath+inputFolderTextEdit.text)
            output = callExec(postProcessCommand)
            outputText.text = outputText.text + "\n" + output

            progressBar.value = 1
            residualImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResidual.png"
            outputImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResult.png"

        }
    }

    function callExec(command) {
        return qLauncher.exec2("sh -c \"" + command + " \"");
    }

    ComboBox {
        id: forwardCombo
        x: 599
        y: 113
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
        x: 599
        y: 166
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
        y: 117
        text: qsTr("MPI Parallel")
    }

    CheckBox {
        id: verboseCheckBox
        x: 843
        y: 166
        text: qsTr("Verbose")
    }

    Text {
        id: residualImageLabel
        x: 64
        y: 226
        text: qsTr("Residual graph")
        font.bold: true
        font.pixelSize: 16
    }

    Text {
        id: outputImageLabel
        x: 599
        y: 226
        text: qsTr("Output Images")
        font.pixelSize: 16
        font.bold: true
    }

    Text {
        id: inputDataLabel
        x: 64
        y: 117
        text: qsTr("Input data")
        font.pixelSize: 16
        font.bold: true
    }

    Text {
        id: parametersLabel
        x: 599
        y: 88
        text: qsTr("Parameters")
        font.pixelSize: 16
        font.bold: true
    }

    TextEdit {
        id: inputFolderTextEdit
        x: 73
        y: 151
        width: 479
        height: 17
        font.pixelSize: 12


        text: qsTr("/home/wouter/Documents/parallelized-fwi/FWIInstall/default")
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
        y: 251
        width: 450
        height: 320
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: outputImage
        x: 599
        y: 251
        width: 450
        height: 320
        fillMode: Image.PreserveAspectFit
    }

    ScrollView {
        id: scrollOutputText
        x: 64
        y: 607
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
        y: 587
        text: qsTr("Output")
        font.bold: true
        font.pixelSize: 16
    }

    Button {
        id: button
        x: 458
        y: 174
        width: 94
        height: 25
        text: qsTr("Select folder")
        onClicked: {
                fileDialog.open()
        }
    }



}
