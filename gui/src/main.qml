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
    color: "#bebebc"

    QPostProcess {
        id: qPostProcess
    }
    QProcess {
        id: qProcess
    }
    QPreProcess {
        id: qPreProcess
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
        value: 0.5
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
            console.log("JS compute clicked")

            preOut = qPreProcess.exec("ls");
            console.log(preOut);
            qProcess.basicPrint();
            qPostProcess.basicPrint();

        }
    }

    ComboBox {
        id: forwardCombo
        x: 599
        y: 113
        width: 167
        height: 40
        flat: true
        currentIndex: -1
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
        currentIndex: -1
        model: ["ConjugateDescent", "GradeientDescent", "EvolutionInversion", "RandomInversion"]
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
        text: qsTr("Text Edit")
        font.pixelSize: 16
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
        source: "qrc:/qtquickplugin/images/template_image.png"
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: outputImage
        x: 599
        y: 251
        width: 470
        height: 350
        source: "qrc:/qtquickplugin/images/template_image.png"
        fillMode: Image.PreserveAspectFit
    }

//    Content Area

//    a button in the middle of the content area
//    Button {
//        text: qsTr("Hello World")
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
//    }

//    Rectangle {
//        x: 17
//        y: 160
//        width: 200
//        height: 100
//        color: "red"

//        Text {
//            anchors.centerIn: parent
//            text: "Hello, World!"
//        }

//        TapHandler {
//            onTapped: parent.color = "blue"
//        }
//    }
}
