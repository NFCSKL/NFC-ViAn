#include "myinputdialog.h"
#include <QKeyEvent>

MyInputDialog::MyInputDialog(QWidget *parnet) : QInputDialog(parnet) {
    setOption(QInputDialog::UsePlainTextEditForTextInput);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void MyInputDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return && e->modifiers() & Qt::ControlModifier) {
        done(1);
        e->accept();
        return;
    }
    e->ignore();
}