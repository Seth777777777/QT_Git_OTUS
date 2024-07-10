#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <QString>
#include <QTimer>


class CustomButton : public QWidget
{
    Q_OBJECT

public:
    explicit CustomButton(const QString &text, QWidget *parent = nullptr);

signals:
    void clicked();
    void pressed();
    void released();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


private:
    QString text;
    bool isPressed;

};

#endif // CUSTOMBUTTON_H
