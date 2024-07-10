#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    MyCircularProgressBar *progressBar;
     TriStateSwitch *switchWidget;
     CustomButton *button;
     CustomButton *button2;  // Вторая кнопка
};

#endif // MAINWINDOW_H
