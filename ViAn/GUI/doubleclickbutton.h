#ifndef DOUBLECLICKBUTTON_H
#define DOUBLECLICKBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class DoubleClickButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DoubleClickButton(QWidget *parent = 0);

private slots:
    void mouseDoubleClickEvent(QMouseEvent *e);

signals:
    void double_clicked();
};

#endif // DOUBLECLICKBUTTON_H
