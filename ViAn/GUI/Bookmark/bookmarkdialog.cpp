#include "bookmarkdialog.h"
#include <QKeyEvent>
#include <QIcon>

BookmarkDialog::BookmarkDialog(QWidget *parnet) : QInputDialog(parnet) {
    setOption(QInputDialog::UsePlainTextEditForTextInput);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon("../ViAn/Icons/bookmark.png"));
    setWindowTitle("Bookmark description");
    setLabelText("Write a description of the bookmark:");
    setToolTip("Write a description of the bookmark. This will be used when creating a report.");
}

void BookmarkDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return && e->modifiers() & Qt::ControlModifier) {
        done(1);
        e->accept();
        return;
    }
    e->ignore();
}
