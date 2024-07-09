#include "custombutton.h"
#include <QMouseEvent>
#include <QDebug>

CustomButton::CustomButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    // Устанавливаем начальный стиль кнопки
    setStyleSheet("background-color: red;");
    // Устанавливаем фиксированный размер кнопки
    setFixedSize(100, 50);
    // Делаем кнопку checkable
    setCheckable(true);

    // Подключаем сигнал для обновления стиля кнопки
    connect(this, &CustomButton::toggled, this, &CustomButton::updateButtonStyle);
}

void CustomButton::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Mouse pressed";  // Отладочное сообщение

    // Вызываем родительский метод для стандартного поведения
    QPushButton::mousePressEvent(event);

    // Обновляем стиль кнопки
    updateButtonStyle();
}

void CustomButton::updateButtonStyle()
{
    if (isChecked()) {
        setStyleSheet("background-color: green;");
    } else {
        setStyleSheet("background-color: red;");
    }
}
