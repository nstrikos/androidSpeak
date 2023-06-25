import QtQuick 2.15
import QtQuick.Window 2.15

import QtQml.StateMachine 1.15 as DSM

Rectangle {
    id: mainRect
    anchors.fill: parent

    Text {
        id: text
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height / 4
        font.family: "Helvetica"
        font.pointSize: 24
        color: "green"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
    }

    Text {
        id: text2
        anchors.top: text.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height / 4
        font.family: "Helvetica"
        font.pointSize: 16
        color: "green"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            console.log("mousearea pressed")
            wake()
        }
    }

//    Rectangle {
//        id: settingsButton
//        border.color: "grey"
//        border.width: 7
//        radius: 15
//        anchors.top:  text2.bottom
//        anchors.topMargin: 50
//        anchors.right: parent.right
//        anchors.rightMargin: parent.width / 3
//        anchors.leftMargin: parent.width / 3
//        anchors.left: parent.left
//        height: 50

//        Text {
//            anchors.top: parent.top
//            anchors.left: parent.left
//            anchors.right: parent.right
//            height: parent.height
//            font.family: "Helvetica"
//            font.pointSize: 16
//            color: "green"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            wrapMode: Text.WordWrap
//            text: qsTr("Settings")
//        }

//        MouseArea {
//            anchors.fill: parent
//            onPressed: {
//                console.log("Settings pressed")
//                showSettings()
//            }
//        }
//    }

    function waitingStateEntered() {
        mainRect.color = "white"
        text.text = "Waiting to connect..."
        text2.text = "If you can't connect:\n1. Enable wifi\n2. Start desktop application"
        visible = true
        //settingsButton.visible = true
    }

    function showInfoStateEntered() {
        mainRect.color = "white"
        //settingsButton.visible = true
        visible = true
        text.text = qsTr("Connected")
        text2.text = "";
    }

    function connectedStateEntered() {
        mainRect.color = "black"
        //settingsButton.visible = false
        text.text = ""
        text2.text = ""
    }

    function setText2(text) {
        text2.text = text
    }
}
