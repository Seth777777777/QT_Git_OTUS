import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 400
    title: qsTr("Circular ProgressBar with Shadow")

    // Корневой элемент
    Rectangle {
        width: 400
        height: 400
        color: "white"
        focus: true

        // Обработчик событий клавиш
        Keys.onPressed: {
            if (event.key === Qt.Key_7) {
                progressBar.value = Math.min(progressBar.value + 1, 100)
            } else if (event.key === Qt.Key_8) {
                progressBar.value = Math.max(progressBar.value - 1, 0)
            }
        }



        // Синий круг
        Rectangle {
            width: 180
            height: 180
            radius: 90
            anchors.centerIn: parent
            color: "#233c4f"
        }

        // Прогрессбар
        Item {
            id: progressBar
            width: 200
            height: 200
            anchors.centerIn: parent
            property int value: 0

            Canvas {
                id: canvas
                width: parent.width
                height: parent.height

                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)

                    ctx.lineWidth = 20
                    ctx.strokeStyle = getProgressBarColor(progressBar.value)
                    ctx.beginPath()
                    ctx.arc(width / 2, height / 2, width / 2 - 20, Math.PI * 1.5, Math.PI * (1.5 + 2 * progressBar.value / 100), false)
                    ctx.stroke()

                    ctx.fillStyle = "black"
                    ctx.font = "bold 20px Arial"
                    ctx.textAlign = "center"
                    ctx.textBaseline = "middle"
                    ctx.fillText(progressBar.value + "%", width / 2, height / 2)
                }

                Component.onCompleted: canvas.requestPaint()
                Connections {
                    target: progressBar
                    onValueChanged: canvas.requestPaint()
                }
            }
        }
    }

    // Функция для определения цвета прогрессбара
    function getProgressBarColor(value) {
        if (value <= 33) {
            var green = 255
            var red = Math.floor(255 * value / 33)
            return "rgb(" + red + "," + green + ",0)"
        } else if (value <= 66) {
            var red = 255
            var green = 255
            var blue = Math.floor(255 * (66 - value) / 33)
            return "rgb(" + red + "," + green + ",0)"
        } else {
            var red = 255
            var green = Math.floor(255 * (100 - value) / 34)
            return "rgb(" + red + "," + green + ",0)"
        }
    }
}
