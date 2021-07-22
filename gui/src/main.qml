import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

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
        y: 765
        width: 1074
        height: 14
        value: 0
    }

    Button {
        id: cancelButton
        x: 902
        y: 708
        text: qsTr("Cancel")
        onClicked: {
            Qt.quit()
        }
    }

    Button {
        id: computeButton
        x: 1038
        y: 708
        text: qsTr("Compute")
        transformOrigin: Item.TopLeft
        onClicked: {
//            console.log(CurDirPath, applicationPath)

            console.log(inputFolderTextEdit.text, forwardCombo.currentText, inversionCombo.currentText)
            progressBar.value = 0.1

            var preProcessCommand = CurDirPath+"../../../FWIInstall/bin/FWI_PreProcess -d '%DATA%' -f %FORWARD%"
            preProcessCommand = preProcessCommand.replace("%DATA%", CurDirPath+inputFolderTextEdit.text)
            preProcessCommand = preProcessCommand.replace("%FORWARD%", forwardCombo.currentText)
            console.log("==== PREPROCESS")
            var output = qLauncher.exec("sh -c \"" + preProcessCommand +" < /dev/null \"");
            console.log(output)
            progressBar.value = 0.33;

            var processCommand = CurDirPath+"../../../FWIInstall/bin/FWI_Process -d '%DATA%' -f %FORWARD% -i %INVERSE%"
            processCommand = processCommand.replace("%DATA%", CurDirPath+inputFolderTextEdit.text)
            processCommand = processCommand.replace("%FORWARD%", forwardCombo.currentText)
            processCommand = processCommand.replace("%INVERSE%", inversionCombo.currentText)
            console.log("==== PROCESS")
            qLauncher.exec(processCommand)
            progressBar.value = 0.66

            console.log("==== POST PROCESS")
            var postProcessCommand = "$(which python3) %BIN%postProcessing-python3.py -o '%DATA%'"
            postProcessCommand = postProcessCommand.replace("%BIN%", CurDirPath+"../../../FWIInstall/")
            postProcessCommand = postProcessCommand.replace("%DATA%", CurDirPath+inputFolderTextEdit.text)
            qLauncher.exec(postProcessCommand);

            progressBar.value = 1

            inputImage.source = applicationPath + inputFolderTextEdit.text + "/output/defaultResidual.png"
            outputImage.source = applicationPath + inputFolderTextEdit.text + "/output/defaultResult.png"


        }
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
        model: ["ConjugateGradient", "GradientDescent", "EvolutionInversion", "RandomInversion"]
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
        id: inputImageLabel
        x: 64
        y: 226
        text: qsTr("Input Image")
        font.bold: true
        font.pixelSize: 16
    }

    Text {
        id: outputImageLabel
        x: 599
        y: 226
        text: qsTr("Output Image")
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
        width: 428
        height: 23
        text: qsTr("../../../FWIInstall/default")
        font.pixelSize: 16


        property string placeholderText: "Enter path/to/data."

        Text {
            text: inputFolderTextEdit.placeholderText
            color: "#aaa"
            visible: !inputFolderTextEdit.text && !inputFolderTextEdit.activeFocus // <----------- ;-)
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
        id: inputImage
        x: 64
        y: 251
        width: 470
        height: 350
//        source: applicationPath + "../../../doc/background/investigationAboutIntegralForwardModelConvergence/Images/conjugateGradient_integral_calcAlphaTrueIntegralForwardModel_templeResult.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: outputImage
        x: 599
        y: 251
        width: 470
        height: 350
        fillMode: Image.PreserveAspectFit
    }

}
