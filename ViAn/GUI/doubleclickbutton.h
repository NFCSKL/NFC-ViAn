#ifndef DOUBLECLICKBUTTON_H
#define DOUBLECLICKBUTTON_H

#include <QPushButton>

class QMouseEvent;
class DoubleClickButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DoubleClickButton(QWidget *parent = nullptr);

private slots:
    void mouseDoubleClickEvent(QMouseEvent *e);

signals:
    void double_clicked();
};

#endif // DOUBLECLICKBUTTON_H
