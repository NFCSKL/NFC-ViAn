#include "myinputdialog.h"
#include <QKeyEvent>
#include <QDebug>

MyInputDialog::MyInputDialog(QWidget *parnet) : QInputDialog(parnet) {
    setOption(QInputDialog::UsePlainTextEditForTextInput);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void MyInputDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return && e->modifiers() & Qt::ControlModifier) {
        qDebug() << "value" << this->textValue();
        done(1);
        e->accept();
        return;
    }
    e->ignore();
}
