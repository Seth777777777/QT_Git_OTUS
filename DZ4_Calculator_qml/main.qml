import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 750
    title: "Calculator"

    property string displayText: "0"
    property double firstOperand: 0
    property double secondOperand: 0
    property string pendingOperation: ""

    Rectangle {
        width: parent.width
        height: parent.height
        color: "lightgray"

        Column {
            spacing: 10
            anchors.fill: parent
            anchors.margins: 10

            Rectangle {
                width: parent.width - 20
                height: 100
                color: "white"
                border.color: "black"
                border.width: 1
                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    id: display
                    text: displayText
                    font.pixelSize: 40
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                    anchors.margins: 10
                    color: "black"
                }
            }

            Item {
                width: parent.width
                height: 20
            }

            Grid {
                columns: 4
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter

                Repeater {
                    model: ["7", "8", "9", "/", "4", "5", "6", "*", "1", "2", "3", "-", "0", ".", "=", "+"]

                    delegate: Button {
                        text: modelData
                        width: 80
                        height: 80
                        font.pixelSize: 24
                        onClicked: handleButtonClick(text)
                        background: Rectangle {
                            width: 80
                            height: 80
                            radius: 40
                            color: control.pressed ? "gray" : "lightblue"
                            border.color: "black"
                            border.width: 2
                        }
                        contentItem: Text {
                            text: modelData
                            anchors.centerIn: parent
                            font.pixelSize: 24
                            color: "black"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }

            Item {
                width: parent.width
                height: 20
            }

            Button {
                text: "C"
                width: parent.width - 20
                height: 80
                font.pixelSize: 24
                onClicked: clear()
                background: Rectangle {
                    width: parent.width - 20
                    height: 80
                    radius: 40
                    color: control.pressed ? "darkred" : "red"
                    border.color: "black"
                    border.width: 2
                }
                contentItem: Text {
                    text: "C"
                    anchors.centerIn: parent
                    font.pixelSize: 24
                    color: "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    function handleButtonClick(buttonText) {
        if (buttonText >= '0' && buttonText <= '9' || buttonText === '.') {
            if (displayText === "0") {
                displayText = buttonText
            } else {
                displayText += buttonText
            }
        } else if (buttonText === "+" || buttonText === "-" || buttonText === "*" || buttonText === "/") {
            firstOperand = parseFloat(displayText)
            pendingOperation = buttonText
            displayText = "0"
        } else if (buttonText === "=") {
            secondOperand = parseFloat(displayText)
            var result = 0
            if (pendingOperation === "+") {
                result = firstOperand + secondOperand
            } else if (pendingOperation === "-") {
                result = firstOperand - secondOperand
            } else if (pendingOperation === "*") {
                result = firstOperand * secondOperand
            } else if (pendingOperation === "/") {
                if (secondOperand !== 0) {
                    result = firstOperand / secondOperand
                } else {
                    result = "Error"
                }
            }
            displayText = result.toString()
        }
    }

    function clear() {
        displayText = "0"
        firstOperand = 0
        secondOperand = 0
        pendingOperation = ""
    }
}
