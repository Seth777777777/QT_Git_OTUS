#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//private slots:
//    void onDigitPressed();
//    void onOperatorPressed();
//    void onEqualPressed();
//    void onClearPressed();

private:
    Ui::MainWindow *ui;
//    QString Operator;
//    double Value;
//    bool Operand;

};
#endif // MAINWINDOW_H
