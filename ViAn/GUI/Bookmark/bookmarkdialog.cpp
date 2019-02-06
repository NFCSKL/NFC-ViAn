#include "bookmarkdialog.h"

#include <QIcon>
#include <QKeyEvent>

BookmarkDialog::BookmarkDialog(QWidget *parent) : QInputDialog(parent) {
    setOption(QInputDialog::UsePlainTextEditForTextInput);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":/Icons/bookmark.png"));
    setWindowTitle("Bookmark description");
    setLabelText("Write a description of the bookmark:");
    setToolTip("Write a description of the bookmark. This will be used when creating a report.");
}

void BookmarkDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return && e->modifiers() & Qt::ControlModifier) {
        done(1);
        e->accept();
        return;
    } else if (e->key() == Qt::Key_Escape) {
        close();
    }
    e->ignore();
}
