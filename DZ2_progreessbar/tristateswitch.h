#ifndef TRISTATESWITCH_H
#define TRISTATESWITCH_H

#include <QWidget>

class TriStateSwitch : public QWidget
{
    Q_OBJECT

public:
    explicit TriStateSwitch(QWidget *parent = nullptr);

signals:
    void stateChanged(int state);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    int state;

    void toggleState();
};

#endif // TRISTATESWITCH_H
