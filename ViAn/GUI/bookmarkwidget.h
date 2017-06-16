#ifndef BOOKMARKWIDGET_H
#define BOOKMARKWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

class BookmarkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookmarkWidget(QWidget *parent = nullptr);
    void add_bookmark(std::string description, std::vector<std::string> tags);

signals:

public slots:

private:
    QListWidget* bookmark_list;

};

#endif // BOOKMARKWIDGET_H
