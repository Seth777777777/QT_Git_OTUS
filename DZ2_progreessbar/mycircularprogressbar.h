#ifndef MYCIRCULARPROGRESSBAR_H
#define MYCIRCULARPROGRESSBAR_H

#include <QWidget>
#include <QColor>

class MyCircularProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit MyCircularProgressBar(QWidget *parent = nullptr);
    void setValue(int value);
    int value() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;


private:
    int m_value;
    QColor calculateColor(int value) const;
};

#endif // MYCIRCULARPROGRESSBAR_H
