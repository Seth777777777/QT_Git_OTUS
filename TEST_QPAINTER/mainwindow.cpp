#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , rectCount(2) // Инициализация количества прямоугольников
{
    ui->setupUi(this);

    // Инициализация массива прямоугольников
    rectArray[0] = QRect(10, 10, 100, 100);
    rectArray[1] = QRect(120, 10, 100, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!painter.isActive()) {
        qWarning("QPainter is not active");
        return;
    }

    // Ваш код для рисования
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRects(rectArray, rectCount);
}
