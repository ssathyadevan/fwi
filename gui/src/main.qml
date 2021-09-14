import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import Qt.labs.settings 1.0
import Qt.labs.folderlistmodel 2.12
import QtQuick.Controls.Styles 1.4

import com.fwi 1.0
import FileIO 1.0


ApplicationWindow {
    visible: true
    id: root
    title: qsTr("Full Waveform Inversion")
    width: 1220
    height: 500
    color: "#e6e6e5"

    Rectangle {
        x: 10
        y: 10
        border.color: "black"
        border.width: 1
        radius: 5
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.fill: parent
    }

    function refreshList(){
        var list = itemsRunList.runList
        listModelData.clear()
        listModelData.append({"name": itemsRunList.runNameList["name"],"Location": itemsRunList.runNameList["Location"],"Threads": itemsRunList.runNameList["Threads"],"Inversion": itemsRunList.runNameList["Inversion"],"Forward": itemsRunList.runNameList["Forward"],"grid": itemsRunList.runNameList["grid"]})
        for(var i in list){
            listModelData.append({"name": list[i]["fileName"],"Location": list[i]["location"],"Threads": list[i]["threads"].toString(),"Inversion": list[i]["inversion"],"Forward": list[i]["forward"],"grid": list[i].ngrid_original.x +"x"+list[i].ngrid_original.z})
        }
        listViewComparison.forceLayout()
    }

    FileIO {
        id: myCSV
        source: "../" + ('0'+itemsRunList.dateObj.getDate()).slice(-2) + ('0' + (itemsRunList.dateObj.getUTCMonth() +1)).slice(-2) + itemsRunList.dateObj.getFullYear().toString() + '_' + itemsRunList.dateObj.getHours() + ':' + itemsRunList.dateObj.getMinutes()  + "_comparison_runs.csv"
        onError: console.log(msg)
    }


    Page {
        id: quePopUpWindow
        anchors.fill: parent
        visible: false


        Item {
            id: itemsRunList
            property var runNameList: {"name":"Run Name","Location":"Location on disk","Threads":"#threads","Inversion":"Inversion","Forward":"Forward","grid":"Grid size"}
            property var runList: []
            property var dateObj: new Date();

            Component.onCompleted: {
                console.log("Current date: " + dateObj.getUTCMonth())
            }
        }

        FileDialog {
            id: fileDialogQueRuns
            title: "Please choose a folder"
            selectFolder: true
            folder: shortcuts.home
            onAccepted: {
                var dir = fileDialogQueRuns.folder.toString()
                try {
                    var json = JSON.parse(openFile(dir+'/input/GenericInput.json'))
                    json.location = dir.replace("file://","")
                    itemsRunList.runList.push(json)
                    refreshList()

                } catch (e) {
                    warningDialogWrongInput.open()
                    console.log(e)
                }
                this.close()
            }
            onRejected: {
                this.close()
            }
        }




        ListView {
            id: listViewComparison
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 60
            delegate: Item {
                x: 5
                width: 1000
                height: 50
                property int indexOfThisDelegate: index
                Row {
                    id: row1
                    spacing: 10

                    Text {
                        width: 80
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: index == 0 ?  true : false
                    }

                    Text {
                        text: grid
                        width: 80
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: index == 0 ?  true : false
                    }
                    Text {
                        text: Forward
                        width: 160
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: index == 0 ?  true : false
                    }
                    Text {
                        text: Inversion
                        width: 160
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: index == 0 ?  true : false

                    }
                    Text {
                        text: Threads
                        width: 60
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: index == 0 ?  true : false
                    }
                    Text {
                        text: Location
                        width: 500
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: index == 0 ?  true : false
                    }
                    Button {
                        id: deleteRow
                        width: 30
                        height: 30
                        visible: index == 0 ? false: true
                        text: qsTr("X")
                        background: Rectangle{
                            implicitWidth: 30
                            implicitHeight: 30
                            border.width: 2
                            border.color: "white"
                            radius: 25
                            color: "light grey"
                        }
                        onClicked: {
                            console.log("Deleted run at index:" + (index - 1) + "for total length:" +itemsRunList.runList.length)
                            itemsRunList.runList.splice(index  -1,1)
                            refreshList()
                        }
                    }
                }
            }
            model: ListModel {id: listModelData}

            Component.onCompleted: {
                refreshList()
            }


        }



        Text {
            id: element
            x: 10
            y: 10
            text: qsTr("FWI Comparison Mode")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            font.bold: true
            font.pixelSize: 26
        }        Rectangle {
            id: rectangle
            width: 200
            color: "#000000"
            anchors.right: listViewComparison.left
            anchors.rightMargin: -5
            anchors.left: listViewComparison.right
            anchors.leftMargin: -5
            anchors.bottom: listViewComparison.top
            anchors.bottomMargin: -5
            anchors.top: listViewComparison.bottom
            anchors.topMargin: -5
        }

        Button {
            id: popUpCompute
            x: 1110
            y: 450
            text: qsTr("Compute")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            background: Rectangle{
                implicitWidth: 100
                implicitHeight: 35
                border.width: 2
                border.color: "grey"
                radius: 4
                color: "#f9013f"
            }
            onClicked: {
                createOuputRunList()
                launchComparisonMode()
                messageDialog.open()
            }
        }

        Button {
            id: addRunButton
            width: 30
            height: 30
            text: qsTr("+")
            font.pointSize: 25
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            background: Rectangle{
                border.width: 2
                border.color: "grey"
                radius: 15
                color: "green"
            }
            onClicked: {
                fileDialogQueRuns.open()
            }
        }

        Button {
            id: popupCancel
            x: 10
            y: 450
            text: qsTr("Cancel")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            onClicked: {
                quePopUpWindow.visible = false
                scrollView.visible = true
            }
            background: Rectangle{
                implicitWidth: 100
                implicitHeight: 35
                border.width: 2
                border.color: "grey"
                radius: 4
                color: "white"
            }
        }

    }


    Launcher {
        id: qLauncher
    }


    MessageDialog {
        id: messageDialog
        icon: StandardIcon.Information
        title: "Full Waveform Inversion"
        text: "The results will be placed in the FWInstall folder."
        width: 300
        standardButtons: StandardButton.Ok
        onAccepted: {

        }
    }

    MessageDialog {
        id: warningDialog
        icon: StandardIcon.Warning
        title: "Save Warning"
        text: "It seems there is already an output for this case folder.

Overwriting the save files creates a mismatch between input and results

Do you want to continue?"
        standardButtons: StandardButton.Yes | StandardButton.Cancel
        onYes:  {
            console.log("Saved File anyway")
            saveJSON(fileDialog.folder.toString())
        }
        onRejected: {
            console.log("Canceled saving due to warning")
        }

    }

    MessageDialog {
        id: warningDialogWrongInput
        icon: StandardIcon.Warning
        title: "Did not detect correct input folder"
        text: "Did not detect 'GenericInput.json'"
        standardButtons: StandardButton.Ok
    }

    function createOuputRunList(){
        var array = []

        for( var i in itemsRunList.runList) {
            array.push(itemsRunList.runList[i].location)
        }
        //Doing some not so-nice conversion between json/myCSV
        var strtxt = JSON.stringify(array).replace("[","").replace("]","").replace(",","\n")
        console.log(strtxt)
        myCSV.write(strtxt)

    }

    function launchComparisonMode() {
        console.log("==== COMPARISON")
        var postProcessCommand = "$(which python3) %BIN%runPerformanceCompare.py -c %DATA%"
        postProcessCommand = postProcessCommand.replace("%BIN%", "./../")
        postProcessCommand = postProcessCommand.replace("%DATA%", "./"+ myCSV.source)
        console.log(postProcessCommand)
        var output = callExec(postProcessCommand)
        console.log(output)
    }


    function openFile(fileUrl) {
        console.log(fileUrl)
        var request = new XMLHttpRequest();
        request.open("GET", fileUrl, false);
        request.send(null);
        return request.responseText;
    }

    function saveFile(fileUrl, text) {
        var request = new XMLHttpRequest();
        request.open("PUT", fileUrl, false);
        request.send(text);
        return request.status;
    }

    function readJSON(path) {
        var input = openFile(path + '/input/GenericInput.json')
        //Try/Catch to see if GenericInput.json is in the right position.
        try {

            var json = JSON.parse(input)
            console.log(json.Freq.min)
            gridLabel2.text = qsTr(String(json.ngrid_original.x) +'x'+String(json.ngrid_original.z))
            currentFigure2.text = qsTr(json.fileName)
            forwardCombo.currentIndex = forwardCombo.find(json.forward)
            inversionCombo.currentIndex = inversionCombo.find(json.inversion)
            numberOfThreads.value = json.threads
            verboseCheckBox.checked = json.verbosity
        }
        catch(error) {
            warningDialogWrongInput.open()
        }
    }

    function saveJSON(path) {
        var input = openFile(path + '/input/GenericInput.json')
        var json = JSON.parse(input)
        json.forward = forwardCombo.currentText
        json.inversion = inversionCombo.currentText
        json.threads = numberOfThreads.value
        json.verbosity = verboseCheckBox.checked
        saveFile(path + '/input/GenericInput.json', JSON.stringify(json))

    }

    function callExec(command) {
        return qLauncher.exec2("sh -c \""  + command + " \"");
    }

    function selectOutputImage() {
        if (outputImageCombo.currentIndex == 0)
        {
            var path = "file://" + inputFolderTextEdit.text + "/output/defaultResult.png"
        }
        else if (outputImageCombo.currentIndex == 1)
        {
            var path = "file://" + inputFolderTextEdit.text + "/output/defaultDummyImage.png"
        }
        else if (outputImageCombo.currentIndex == 2)
        {
            var path  = "file://" + inputFolderTextEdit.text + "/output/defaultChiDifference.png"
        }
        outputImage.source = path ? path: ""
        residualImage.source = "file://" + inputFolderTextEdit.text + "/output/defaultResidual.png" ? "file://" + inputFolderTextEdit.text + "/output/defaultResidual.png": ""
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")
                onTriggered: fileDialog.open()
            }
            Action {
                text: qsTr("&Save")
                onTriggered: {
                    //Check if output exists, if so, display warning before saving
                    if (outputImageCombo === "") {
                        saveJSON(fileDialog.folder.toString())
                    } else {
                        warningDialog.open()
                    }
                }
            }

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

                }
            }
        }
    }


    ScrollView {
        id: scrollView
        height: 600
        visible: true
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        contentHeight: 1050  // Same
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        FileDialog {
            id: fileDialog
            title: "Please choose a folder"
            selectFolder: true
            folder: shortcuts.home
            onAccepted: {
                inputFolderTextEdit.text = fileDialog.folder.toString().replace("file://","")
                readJSON(fileDialog.folder.toString())
                selectOutputImage()
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
                text: qsTr("Compare runs")
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
                color: "#f9013f"
            }
            onClicked: {
                quePopUpWindow.visible = true
                scrollView.visible = false
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
                saveJSON(fileDialog.folder.toString())
                progressBar.value = 0
                residualImage.source = ""
                outputImage.source = ""

                console.log(CurDirPath)
                console.log(inputFolderTextEdit.text, forwardCombo.currentText, inversionCombo.currentText)
                progressBar.value = 0.1

                if (preProcessingCheckBox.checked)
                {
                    console.log("==== PREPROCESS")
                    var preProcessCommand = "./FWI_PreProcess -d '%DATA%'"
                    preProcessCommand = preProcessCommand.replace("%DATA%", inputFolderTextEdit.text)
                    var output = callExec(preProcessCommand)
                    console.log(preProcessCommand)
                    outputText.text = output
                }
                progressBar.value = 0.33

                if (processingCheckBox.checked)
                {
                    console.log("==== PROCESS")
                    var processCommand = "./FWI_Process -d '%DATA%'"
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
                    postProcessCommand = postProcessCommand.replace("%BIN%", "./../")
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
            id: numberOfThreads
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
            ToolTip.visible: hovered
            ToolTip.text: qsTr(" Hello there UwU")
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
            property string placeholderText: "Enter path/to/data/default (containing input&output folders)."

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
            y: 720
            width: 955
            height: 127
            border.width: 1
            border.color: "grey"
            radius: 3
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 50
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

        Text {
            id: gridLabel
            x: 64
            y: 150
            text: qsTr("Grid Size: ")
            font.pixelSize: 12
            font.bold: true
        }
        Text {
            id: gridLabel2
            x: 64
            y: 170
            font.pixelSize: 12

        }

        Text {
            id: currentFigure
            x: 164
            y: 150
            text: qsTr("Subsurface Model: ")
            font.pixelSize: 12
            font.bold: true
        }
        Text {
            id: currentFigure2
            x: 164
            y: 170
            font.pixelSize: 12

        }


        CheckBox {
            id: preProcessingCheckBox
            x: 64
            y: 230
            text: qsTr("Pre-processing")
            checkState: Qt.Checked
            tristate: false
            checked: false
        }

        CheckBox {
            id: processingCheckBox
            x: 264
            y: 230
            text: qsTr("Processing")
            checkState: Qt.Checked
            checked: false
        }

        CheckBox {
            id: postProcessingCheckBox
            x: 424
            y: 230
            text: qsTr("Post-processing")
            checkState: Qt.Checked
            checked: false
        }


    }






}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:1;invisible:true}D{i:3;anchors_height:398;anchors_width:1190;anchors_x:15;anchors_y:51}
D{i:13;anchors_x:10;anchors_y:5}D{i:15;anchors_x:10}D{i:16;anchors_height:200;anchors_x:555;anchors_y:150}
D{i:18;anchors_height:200;anchors_x:555;anchors_y:150}D{i:2;anchors_height:500;anchors_width:1220}
D{i:33;invisible:true}D{i:74;anchors_y:10}D{i:29;invisible:true}D{i:31;invisible:true}
}
##^##*/
