#include "doubleclickbutton.h"

#include <QMouseEvent>

DoubleClickButton::DoubleClickButton(QWidget *parent) :
    QPushButton(parent) {
}

void DoubleClickButton::mouseDoubleClickEvent(QMouseEvent *e) {
    Q_UNUSED(e)
    emit double_clicked();
}
