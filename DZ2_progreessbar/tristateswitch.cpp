#include "tristateswitch.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

TriStateSwitch::TriStateSwitch(QWidget *parent)
    : QWidget(parent), state(0)
{
    setFixedSize(60, 30);
    setMouseTracking(true);
}

void TriStateSwitch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect = this->rect();

    QColor color;
    if (state == 0)
        color = QColor(200, 200, 200);  // Grey for unset
    else if (state == 1)
        color = QColor(0, 255, 0);  // Green for set
    else
        color = QColor(255, 255, 0);  // Yellow for tristate

    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 15, 15);

    // Draw the circle (the switch)
    QRect circleRect;
    if (state == 0)
        circleRect = QRect(5, 5, 20, 20);
    else if (state == 1)
        circleRect = QRect(35, 5, 20, 20);
    else
        circleRect = QRect(20, 5, 20, 20);

    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(circleRect);
}

void TriStateSwitch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        toggleState();
        emit stateChanged(state);
        update();
    }
}

void TriStateSwitch::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        toggleState();
        emit stateChanged(state);
        update();
    }
}

void TriStateSwitch::toggleState()
{
    state = (state + 1) % 3;
}
