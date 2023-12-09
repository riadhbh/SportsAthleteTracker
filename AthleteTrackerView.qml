import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 922
    height: 261
    title: "AthleteTracker"

    Rectangle {
        id: centralWidget
        anchors.fill: parent

        Button {
            id: browseBtn
            text: "Browse..."
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.topMargin: 50
            width: 80
            height: 25
        }

        Button {
            id: processBtn
            text: "Process"
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.topMargin: 170
            width: 80
            height: 25
        }

        Label {
            id: filePathLabel
            anchors.left: parent.left
            anchors.leftMargin: 190
            anchors.topMargin: 50
            width: 591
            height: 21
            text: ""
        }

        Button {
            id: playBtn
            text: "Play Video"
            anchors.left: parent.left
            anchors.leftMargin: 410
            anchors.topMargin: 170
            width: 80
            height: 25
            checkable: true
        }

        Button {
            id: openLogBtn
            text: "Open log file"
            anchors.left: parent.left
            anchors.leftMargin: 790
            anchors.topMargin: 170
            width: 80
            height: 25
        }

        ProgressBar {
            id: progressBar
            anchors.left: parent.left
            anchors.leftMargin: 160
            anchors.topMargin: 110
            width: 631
            height: 23
            value: 0
        }
    }

    MenuBar {
        id: menubar
    }
}
