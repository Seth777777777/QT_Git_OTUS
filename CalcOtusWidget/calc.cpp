#include "calc.h"
#include "ui_calc.h"

Calc::Calc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calc)
{
    ui->setupUi(this);

    connect(ui->pushButton_0, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_1, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_2, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_3, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_4, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_5, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_6, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_7, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_8, &QPushButton::clicked, this , &Calc::onDigitPressed);
    connect(ui->pushButton_9, &QPushButton::clicked, this , &Calc::onDigitPressed);

    connect(ui->pushButton_plus, &QPushButton::clicked, this , &Calc::onOperatorPressed);
    connect(ui->pushButton_minus, &QPushButton::clicked, this , &Calc::onOperatorPressed);
connect(ui->pushButton_devide, &QPushButton::clicked, this , &Calc::onOperatorPressed);
    connect(ui->pushButton_mult, &QPushButton::clicked, this , &Calc::onOperatorPressed);
connect(ui->pushButton_equal, &QPushButton::clicked, this , &Calc::onEqualPressed);
connect(ui->pushButton_dot, &QPushButton::clicked, this , &Calc::onClearPressed);

}

Calc::~Calc()
{
    delete ui;
}

void Calc::onDigitPressed() {

    QPushButton *button = qobject_cast<QPushButton *> (sender());
    QString digitValue = button->text();
    QString displayValue = ui->label->text();

    if(displayValue == "0" || Operand ){

        ui->label->setText(digitValue);
        Operand = false;
    }
    else
    {

        ui->label->setText(displayValue+digitValue);
    }

}



void Calc::onOperatorPressed(){
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString operatorValue = button->text();
    Value = ui->label->text().toDouble();
    Operator =  operatorValue;
    Operand = true;


}
void Calc::onEqualPressed(){
    double calcValue = ui->label->text().toDouble();
    double result = 0.0;

    if(Operator == "+")
    {
        result = calcValue+ Value;
    }
    else if(Operator == "-")
    {
        result = Value-calcValue;
    }
    else if(Operator == "*")
    {
                result= Value*calcValue;
    }

    else if (Operator == "/")
    {
        if (calcValue == 0.0) {
            ui->label->setText("Ошибка");
            return;

        }
        result = Value / calcValue;

    }

    ui->label->setText(QString::number(result));
    Operand = false;


}
void Calc::onClearPressed(){

    ui->label->setText("0");
    Operand = true;

}
