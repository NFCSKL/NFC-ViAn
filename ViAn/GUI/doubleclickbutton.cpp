#include "doubleclickbutton.h"
#include <QDebug>

DoubleClickButton::DoubleClickButton(QWidget *parent) :
    QPushButton(parent) {
}

void DoubleClickButton::mouseDoubleClickEvent(QMouseEvent *e) {
    emit double_clicked();
}
