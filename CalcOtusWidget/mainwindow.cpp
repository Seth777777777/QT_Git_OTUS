#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);




}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::onDigitPressed() {


//}



//void MainWindow::onOperatorPressed(){


//}
//void MainWindow::onEqualPressed(){


//}
//void MainWindow::onClearPressed(){


//}
