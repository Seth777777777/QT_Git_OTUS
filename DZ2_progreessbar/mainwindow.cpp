#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    progressBar = new MyCircularProgressBar(this);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(progressBar);


    // Устанавливаем фокус на прогрессбар при создании окна
    progressBar->setFocus();
}
