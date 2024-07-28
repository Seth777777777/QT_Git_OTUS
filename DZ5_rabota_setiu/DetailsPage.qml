import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property string path

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "Back"
            width: 200
            height: 40
            font.pixelSize: 18
            background: Rectangle {
                color: "lightblue"
                radius: 10
                border.color: "blue"
                border.width: 2
            }
            onClicked: {
                stackView.pop()
            }
        }

        Text {
            id: responseLabel
            width: parent.width
            font.pixelSize: 16
            color: "black"
        }
    }

    Connections {
        target: jsonHandler
        function onServerResponse(response) {
            responseLabel.text = response;
        }
    }
}
