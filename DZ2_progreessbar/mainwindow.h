#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "MyCircularProgressBar.h"
#include "tristateswitch.h"
#include "custombutton.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onButtonClicked();
    void onButtonDec();
    void onSwitchStateChanged(int state);
    void onIncrementButtonPressed();
    void onDecrementButtonPressed();
    void onButtonReleased();
    void onIncrementTimeout();
    void onDecrementTimeout();


private:
    MyCircularProgressBar *progressBar;
     TriStateSwitch *switchWidget;
     CustomButton *button;
     CustomButton *button2;  // Вторая кнопка

     QTimer incrementTimer;
     QTimer decrementTimer;

     bool incrementButtonPressed;
     bool decrementButtonPressed;
};

#endif // MAINWINDOW_H
