import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 1024
    height: 768

    Material.theme: Material.Light
    Material.accent: Material.Blue

    StackView {
        id: stackView
        initialItem: homePage
        anchors.fill: parent
    }

    Component.onCompleted: {
        jsonHandler.fetchData()
    }

    Component {
        id: homePage
        Item {
            anchors.fill: parent

            Column {
                anchors.fill: parent
                spacing: 20

                Text {
                    text: "Главный экран"
                    font.pixelSize: 24
                    color: "blue"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 20
                }

                ListView {
                    id: buttonListView
                    width: parent.width
                    height: parent.height - 100
                    anchors.top: parent.top
                    anchors.topMargin: 80
                    model: servicesModel

                    delegate: Item {
                        width: buttonListView.width
                        height: 100

                        Column {
                            spacing: 10
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top

                            Text {
                                text: model.name
                                font.pixelSize: 20
                                color: "black"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Text {
                                text: model.description
                                font.pixelSize: 16
                                color: "gray"
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            Button {
                                text: model.name
                                width: 200
                                height: 40
                                font.pixelSize: 18
                                anchors.horizontalCenter: parent.horizontalCenter
                                onClicked: {
                                    console.log("Fetching data with path: " + model.path)
                                    if (model.name === "goods") {
                                        jsonHandler.fetchDataWithPath(model.path)
                                    } else {
                                        jsonHandler.fetchDataWithPath(model.path)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: detailsPage
        Item {
            anchors.fill: parent

            Column {
                anchors.fill: parent
                spacing: 20

                Text {
                    text: "Детали"
                    font.pixelSize: 24
                    color: "blue"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: "Координаты: " + JSON.stringify(jsonHandler.coord)
                }
                Text {
                    text: "Погода: " + JSON.stringify(jsonHandler.weather)
                }
                Text {
                    text: "Основная информация: " + JSON.stringify(jsonHandler.main)
                }
                Text {
                    text: "Видимость: " + jsonHandler.visibility
                }
                Text {
                    text: "Ветер: " + JSON.stringify(jsonHandler.wind)
                }
                Text {
                    text: "Дождь: " + JSON.stringify(jsonHandler.rain)
                }
                Text {
                    text: "Облачность: " + JSON.stringify(jsonHandler.clouds)
                }
                Text {
                    text: "Система: " + JSON.stringify(jsonHandler.sys)
                }
                Text {
                    text: "Часовой пояс: " + jsonHandler.timezone
                }
                Text {
                    text: "ID: " + jsonHandler.id
                }
                Text {
                    text: "Название: " + jsonHandler.name
                }
                Text {
                    text: "Код: " + jsonHandler.cod
                }

                Rectangle {
                    width: 200
                    height: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    Button {
                        text: "Back"
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        onClicked: {
                            stackView.pop()
                            jsonHandler.fetchData() // Перезагружаем данные при возврате на главный экран
                        }
                    }
                }
            }
        }
    }

    Component {
        id: goodsPage
        Item {
            anchors.fill: parent

            Column {
                anchors.fill: parent
                spacing: 20

                Text {
                    text: "Товары"
                    font.pixelSize: 24
                    color: "blue"
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                ListView {
                    id: goodsListView
                    width: parent.width
                    height: parent.height - 150
                    anchors.top: parent.top
                    anchors.topMargin: 60
                    model: jsonHandler.goods

                    delegate: Item {
                        width: goodsListView.width
                        height: 200 // Увеличен размер для удобного отображения данных

                        Column {
                            spacing: 10
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top

                            Text {
                                text: modelData.name !== undefined ? modelData.name : "No name"
                                font.pixelSize: 20
                                color: "black"
                            }
                            Text {
                                text: "Цена: " + (modelData.price !== undefined ? modelData.price : "N/A") + " Руб"
                                font.pixelSize: 16
                                color: "gray"
                            }
                            Text {
                                text: "Цена со скидкой: " + (modelData.discount_price !== undefined ? modelData.discount_price : (modelData.discont_price !== undefined ? modelData.discont_price : "N/A")) + " Руб"
                                font.pixelSize: 16
                                color: "red"
                            }
                            Text {
                                text: "Количество: " + (modelData.quantity !== undefined ? modelData.quantity : "N/A")
                                font.pixelSize: 16
                                color: "gray"
                            }
                            Text {
                                text: "Избранное: " + (modelData.in_favorite !== undefined ? (modelData.in_favorite ? "Да" : "Нет") : "N/A")
                                font.pixelSize: 16
                                color: modelData.in_favorite ? "green" : "red"
                            }
                            Text {
                                text: "Ссылка на изображение: " + (modelData.image !== undefined ? modelData.image : "No image")
                                font.pixelSize: 16
                                color: "blue"
                            }
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "lightgray"
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.bottom
                            }
                        }
                    }
                }

                Rectangle {
                    width: 200
                    height: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom // Переместим кнопку "Back" вниз
                    Button {
                        text: "Back"
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        onClicked: {
                            stackView.pop()
                            jsonHandler.fetchData() // Перезагружаем данные при возврате на главный экран
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: servicesModel
    }

    Connections {
        target: jsonHandler
        function onDataFetched(services) {
            servicesModel.clear()
            for (var i = 0; i < services.length; i++) {
                servicesModel.append(services[i])
            }
        }
    }

    Connections {
        target: jsonHandler
        onDetailDataFetched: {
            stackView.push(detailsPage)
        }
    }

    Connections {
        target: jsonHandler
        onGoodsDataFetched: {
            console.log("Goods data fetched, navigating to goods page");
            console.log("Goods data: " + JSON.stringify(jsonHandler.goods));
            stackView.push(goodsPage)
        }
    }
}
