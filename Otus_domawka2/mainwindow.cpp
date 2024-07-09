#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "custombutton.h"
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , button(nullptr)  // Инициализируем button как nullptr
{
    ui->setupUi(this);

    // Создаем экземпляр пользовательской кнопки
    button = new CustomButton("Click Me", this);

    // Устанавливаем позицию кнопки
    button->move(100, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed:" << event->key();  // Отладочное сообщение

    if (event->key() == Qt::Key_2) {
        if (button) {  // Проверяем, что button не nullptr
            qDebug() << "Toggling button";  // Отладочное сообщение
            button->toggle();  // Включение/выключение кнопки
        } else {
            qDebug() << "Button is not initialized!";  // Отладочное сообщение
        }
    }
}
