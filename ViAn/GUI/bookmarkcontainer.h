#ifndef BOOKMARKCONTAINER_H
#define BOOKMARKCONTAINER_H
#include <QWidget>
#include <QListWidget>
#include <QLayout>
class BookmarkContainer : public QListWidget
{
    Q_OBJECT
    QVBoxLayout* layout;
public:
    explicit BookmarkContainer(QWidget* parent = nullptr);

};

#endif // BOOKMARKCONTAINER_H
