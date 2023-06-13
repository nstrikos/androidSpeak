import QtQuick 2.15
import QtQuick.Window 2.15

import QtQml.StateMachine 1.15 as DSM

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Omilo android")

    signal countDownFinished()
    signal connectionEstablished()
    signal disconnected()
    signal wake()
    signal showSettings()
    signal exitSettings()

    property int secondsElapsed: 0

    property bool isConnected: false

    States {
    }

    Connections {
        target: chat
        function onConnected() {
            console.log("connected")
            isConnected = true
            connectionEstablished()
        }
    }

    Connections {
        target: chat
        function onDisconnected() {
            console.log("disconnected")
            isConnected = false
            disconnected()
            texttospeech.stop()
        }
    }

    MainRect {
        id: mainRect
    }

    SpeechSettings {
        id: speechSettings
        anchors.fill: parent
        visible: false
    }

    Timer {
        id:timer
        interval: 1000
        repeat: true
        onTriggered: {
            secondsElapsed++
            if (secondsElapsed >= 10) {
                countDownFinished()
            } else {
                mainRect.setText2(qsTr("The screen will turn off in ") + (10 - secondsElapsed))
                timer.repeat = true
                timer.start()
            }
        }
    }

    function waitingStateEntered() {
        console.log("waiting state")
        timer.repeat = false
        timer.stop()
        secondsElapsed = 0
        mainRect.waitingStateEntered()
    }

    function settingsStateEntered() {
        console.log("settings state")
        speechSettings.visible = true
        mainRect.visible = false
    }

    function settingsStateExited() {
        console.log("settingsState exited")
        speechSettings.visible = false
    }

    function showInfoStateEntered() {
        console.log("show info state")
        timer.repeat = false
        timer.stop()
        secondsElapsed = 0
        mainRect.showInfoStateEntered()
        timer.start()
    }

    function showInfoStateExited() {
        console.log("Show info state exited")
        timer.repeat = false
        timer.stop()
    }

    function connectedStateEntered() {
        console.log("connected state")
        mainRect.connectedStateEntered()
    }
}
