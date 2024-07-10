#include "MainWindow.h"
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    progressBar = new MyCircularProgressBar(this);
     switchWidget = new TriStateSwitch(this);
     button = new CustomButton("Increment", this);
     button2 = new CustomButton("Decrement",this);

//    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttonlayout = new QHBoxLayout;
    QHBoxLayout *tristatelayout = new QHBoxLayout;

    buttonlayout->addWidget(button);
    buttonlayout->addWidget(button2);

    tristatelayout->addStretch();
    tristatelayout->addWidget(switchWidget);
    tristatelayout->addStretch();

    layout->addWidget(progressBar);
//    layout->addWidget(switchWidget);
    layout->addLayout(tristatelayout);
    layout->addLayout(buttonlayout);
//    layout->addWidget(button);
//    layout->addWidget(button2);




    connect(button, &CustomButton::clicked, this, &MainWindow::onButtonClicked);
    connect(button2, &CustomButton::clicked,this, &MainWindow::onButtonDec);
    connect(switchWidget , &TriStateSwitch::stateChanged, this, &MainWindow::onSwitchStateChanged);

    connect(button, &CustomButton::pressed, this, &MainWindow::onIncrementButtonPressed);
    connect(button, &CustomButton::released, this, &MainWindow::onButtonReleased);
    connect(button2, &CustomButton::pressed, this, &MainWindow::onDecrementButtonPressed);
    connect(button2, &CustomButton::released, this, &MainWindow::onButtonReleased);

    connect(&incrementTimer, &QTimer::timeout, this, &MainWindow::onIncrementTimeout);
    connect(&decrementTimer, &QTimer::timeout, this, &MainWindow::onDecrementTimeout);

    incrementTimer.setInterval(100);
    decrementTimer.setInterval(100);


    // Устанавливаем фокус на прогрессбар при создании окна
    progressBar->setFocus();

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    resize(300,400);

//    switchWidget = new TriStateSwitch(this);
//       setCentralWidget(switchWidget);
//       resize(300, 200);
}


void MainWindow::onButtonClicked()
{

    int currentProgress = progressBar->value();
    progressBar->setValue((currentProgress + 1) % 101);
}

void MainWindow::onButtonDec()
{

    int currentProgress = progressBar->value();
    if (currentProgress > 0) {
    progressBar->setValue(currentProgress - 1);
    }
}

void MainWindow::onSwitchStateChanged(int state)
{
    if (state == 0) {
        progressBar->setValue(33);
    } else if (state == 1) {
        progressBar->setValue(66);
    } else if (state == 2) {
        progressBar->setValue(100);
    }
}

void MainWindow::onIncrementButtonPressed()
{
    incrementButtonPressed = true;
    QTimer::singleShot(1000, this, [this]() {
        if (incrementButtonPressed) {
            incrementTimer.start();
        }
    });
}

void MainWindow::onDecrementButtonPressed()
{
    decrementButtonPressed = true;
    QTimer::singleShot(1000, this, [this]() {
        if (decrementButtonPressed) {
            decrementTimer.start();
        }
    });
}

void MainWindow::onButtonReleased()
{
    incrementButtonPressed = false;
    decrementButtonPressed = false;
    incrementTimer.stop();
    decrementTimer.stop();
}

void MainWindow::onIncrementTimeout()
{
    int currentProgress = progressBar->value();
    progressBar->setValue((currentProgress + 1) % 101);
}

void MainWindow::onDecrementTimeout()
{
    int currentProgress = progressBar->value();
    if (currentProgress > 0) {
        progressBar->setValue(currentProgress - 1);
    }
}
