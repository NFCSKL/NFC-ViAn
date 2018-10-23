#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QInputDialog>

class BookmarkDialog : public QInputDialog {
    Q_OBJECT

public:
    explicit BookmarkDialog(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent *e) override;

};

#endif // BOOKMARKDIALOG_H
