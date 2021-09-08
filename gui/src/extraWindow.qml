import QtQuick 2.0
import QtQuick.Window 2.12
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles.Desktop 1.0
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.5

Rectangle {
    id: childWindow
    width: 400
    height: 300
    color: "#909392"

    TextArea {
        id: textArea
        anchors.fill: parent
        text: qsTr("About

FWI project dingen

extra window openen")
        anchors.leftMargin: 10
        anchors.topMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        Button {
            id: button
            x: 272
            y: 232
            text: qsTr("Close")
            onClicked: {
                Qt.quit()
            }
        }
    }
    // stuff
}
