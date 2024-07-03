#ifndef CALC_H
#define CALC_H

#include <QWidget>

namespace Ui {
class Calc;
}

class Calc : public QWidget
{
    Q_OBJECT

public:
    explicit Calc(QWidget *parent = nullptr);
    ~Calc();

private slots:
    void onDigitPressed();
    void onOperatorPressed();
    void onEqualPressed();
    void onClearPressed();

private:
    Ui::Calc *ui;
    QString Operator;
    double Value;
    bool Operand;
};

#endif // CALC_H
