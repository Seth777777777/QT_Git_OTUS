#include "MyCircularProgressBar.h"
#include <QPainter>
#include <QConicalGradient>
#include <QKeyEvent>
#include <QDebug>

MyCircularProgressBar::MyCircularProgressBar(QWidget *parent)
    : QWidget(parent), m_value(0)
{
    setMinimumSize(200, 200);
}

void MyCircularProgressBar::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        update(); // Перерисовать виджет при изменении значения
    }
}

int MyCircularProgressBar::value() const
{
    return m_value;
}

QColor MyCircularProgressBar::calculateColor(int value) const
{
    if (value <= 33) {
        int green = 255;
        int red = static_cast<int>(255.0 * value / 33.0);
        return QColor(red, green, 0);
    } else if (value <= 66) {
        int red = 255;
        int green = 255;
        int blue = static_cast<int>(255.0 * (66 - value) / 33.0);
        return QColor(red, green, 0);
    } else {
        int red = 255;
        int green = static_cast<int>(255.0 * (100 - value) / 34.0);
        return QColor(red, green, 0);
    }
}

void MyCircularProgressBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

   // QRectF rect(10, 10, width() - 20, height() - 20);


    // Создать прямоугольник для дуги прогресса с измененным радиусом
    int padding = 10; // Измените значение padding для изменения радиуса дуги
    QRectF rect(padding, padding, width() - 2 * padding, height() - 2 * padding);

    // Нарисовать синий круг
    painter.setBrush(QColor("#172734"));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(rect.adjusted(-10, -10, 10, 10));

    //QRectF lowerRect(10, height() - 30, width() - 20, height() - 20);

    // Отрисовать внешний круг
   // painter.drawEllipse(rect);

    // Отрисовать верхний прогресс бар
    QPen pen;
    pen.setWidth(15);
    pen.setColor(calculateColor(m_value));
    painter.setPen(pen);

    int startAngle = 270 * 16; // Начало на 19 часах (270 градусов)
    int spanAngle = -static_cast<int>(m_value * 3.6 * 16);

    painter.drawArc(rect, startAngle, spanAngle);



    // Отрисовать текст внутри круга
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(16);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString("%1%").arg(m_value));
}

void MyCircularProgressBar::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key Pressed:" << event->key();
    if (event->key() == Qt::Key_7) {
        setValue(qMin(m_value + 1, 100)); // Увеличить значение на 5, но не больше 100
    } else if (event->key() == Qt::Key_8) {
        setValue(qMax(m_value - 1, 0)); // Уменьшить значение на 5, но не меньше 0
    }
}

void MyCircularProgressBar::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "ProgressBar received focus";
}
