import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQml.Models 2.15

Item {
    id: root
    visible: false
    width: 640
    height: 480

    focus: true

    property string text: qsTr("Omilo android 1 2 3 4 5 6 7 8 9 10")


    Keys.onBackPressed: {
        backPressed()
    }

    Keys.onEscapePressed: {
        backPressed()
    }

    function backPressed() {
        window.exitSettings()
    }

    function setActiveFocus() {
        slider1.forceActiveFocus()
    }

    ColumnLayout {
        spacing: 25

        Row {
            id: row1
            Layout.fillWidth: true
            Label {
                id: label1
                text: qsTr("Volume: ") + slider1.value
                width: 150
                font.pointSize: 20
                anchors.verticalCenter: row1.verticalCenter
            }
            Slider {
                id: slider1
                anchors.verticalCenter: row1.verticalCenter
                width: window.width - label1.width
                value: 50
                stepSize: 1
                from: 0
                to: 100
                onValueChanged:  {
                    texttospeech.volume = value
                    label1.text = qsTr("Volume: ") + slider1.value
                    if (activeFocus && window.talkSpeech)
                        texttospeech.speak(label1.text)
                }
                onFocusChanged: {
                    if (activeFocus && window.talkSpeech) {
                        texttospeech.speak(label1.text)
                    }
                }
            }
        }

        Row {
            id: row2
            Layout.fillWidth: true
            Label {
                id: label2
                text: qsTr("Rate: ") + slider2.value
                width: 150
                font.pointSize: 20
                anchors.verticalCenter: row2.verticalCenter
            }
            Slider {
                id: slider2
                anchors.verticalCenter: row2.verticalCenter
                width: window.width - label2.width
                value: 50
                stepSize: 1
                from: -100
                to: 100
                onValueChanged:  {
                    texttospeech.rate = value
                    label2.text = qsTr("Rate: ") + slider2.value
                    if (activeFocus && window.talkSpeech)
                        texttospeech.speak(label2.text)
                }
                onFocusChanged: {
                    if (activeFocus && window.talkSpeech) {
                        texttospeech.speak(label2.text)
                    }
                }
            }
        }

        Row {
            id: row3
            Layout.fillWidth: true
            Label {
                id: label3
                text: qsTr("Pitch: ") + slider3.value
                width: 150
                anchors.verticalCenter: row3.verticalCenter
                font.pointSize: 20
            }
            Slider {
                id: slider3
                anchors.verticalCenter: row3.verticalCenter
                width: window.width - label3.width
                value: 50
                stepSize: 1
                from: -100
                to: 100
                onValueChanged:  {
                    texttospeech.pitch = value
                    label3.text = qsTr("Pitch: ") + slider3.value
                    if (activeFocus && window.talkSpeech)
                        texttospeech.speak(label3.text)
                }
                onFocusChanged: {
                    if (activeFocus && window.talkSpeech) {
                        texttospeech.speak(label3.text)
                    }
                }
            }
        }

        Row {
            width: window.width
            Button {
                id: button
                property bool isActive: false
                property bool isPressed: false
                text: qsTr("Test speech")
                width: window.width
                height: 80
                font.pointSize: 25
                onPressed:  {
                    texttospeech.speak("1 2 3 4 5 6 7 8 9 10")
                }

                background: Rectangle {
                    implicitHeight: 100
                    radius: 10
                    border.color: {
                        if (button.isActive)
                            return "red"
                        else
                            return "gray"
                    }
                    color: button.isActive ? "lightblue": "white"
                    border.width: 5
                }

                onFocusChanged:     if (activeFocus) {
                                        isActive = true
                                        if (window.talkSpeech)
                                            texttospeech.speak(button.text)
                                    }
                                    else {
                                        isActive = false
                                    }

            }
        }

        Row {
            width: window.width
            Button {
                id: button2
                property bool isActive: false
                property bool isPressed: false
                text: qsTr("Exit")
                width: window.width
                height: 40
                font.pointSize: 25
                onPressed:  {
                    //texttospeech.speak(textEdit.text)
                    window.exitSettings()
                }

                background: Rectangle {
                    implicitHeight: 100
                    radius: 10
                    border.color: {
                        if (button2.isActive)
                            return "red"
                        else
                            return "gray"
                    }
                    color: button2.isActive ? "lightblue": "white"
                    border.width: 5
                }

                onFocusChanged:     if (activeFocus) {
                                        isActive = true
                                        if (window.talkSpeech)
                                            texttospeech.speak(button2.text)
                                    }
                                    else {
                                        isActive = false
                                    }

            }
        }
    }

    Component.onCompleted: {
        slider1.value = Math.round(texttospeech.volume * 100)
        slider2.value = Math.round(texttospeech.rate * 100)
        slider3.value = Math.round(texttospeech.pitch * 100)
    }
}
