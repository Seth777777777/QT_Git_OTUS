#include "custombutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

CustomButton::CustomButton(const QString &text, QWidget *parent)
    : QWidget(parent), text(text), isPressed(false)
{
    setFixedSize(150, 50);

}

void CustomButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = this->rect();

    QColor backgroundColor = isPressed ? QColor(155, 0, 0) : QColor(0, 200, 0);
    //QColor textColor = isPressed ? QColor(255, 255, 255) : QColor(0, 0, 0);
    QColor textColor = QColor(255, 255, 255);

    painter.setBrush(QBrush(backgroundColor));
    painter.setPen(Qt::NoPen);
    //painter.drawRect(rect);
    painter.drawRoundedRect(rect, 10, 10);

    painter.setPen(textColor);
    painter.drawText(rect, Qt::AlignCenter, text);
}

void CustomButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPressed = true;
        emit pressed();
        update();
    }
}

void CustomButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (isPressed && event->button() == Qt::LeftButton) {
        isPressed = false;
        emit released();
        emit clicked();
        update();
    }
}

void CustomButton::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) {
        isPressed = true;
        emit pressed();
        update();
    }
}

void CustomButton::keyReleaseEvent(QKeyEvent *event)
{
    if (isPressed && (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return)) {
        isPressed = false;
        emit released();
        emit clicked();
        update();
    }
}
