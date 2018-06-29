#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QInputDialog>

class BookmarkDialog : public QInputDialog {
    Q_OBJECT

public:
    explicit BookmarkDialog(QWidget* parnet = 0);
    void keyPressEvent(QKeyEvent *e) override;

signals:

public slots:

};

#endif // BOOKMARKDIALOG_H
