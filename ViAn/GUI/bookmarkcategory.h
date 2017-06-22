#ifndef BOOKMARKFOLDER_H
#define BOOKMARKFOLDER_H
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QDebug>
#include <QScrollArea>
#include <QCommonStyle>
#include "bookmarkwidget.h"
#include "bookmarkcontainer.h"
/**
 * @brief The BookmarkCategory class
 * This class is inteded to store two scrollable lists
 * containing bookmars or images.
 */
class BookmarkCategory : public QObject, public QListWidgetItem
{
    Q_OBJECT
    QVBoxLayout*  layout = nullptr;
    QScrollArea* disputed = nullptr;
    QScrollArea* reference = nullptr;
public:
    explicit BookmarkCategory(QListWidget *parent = nullptr);
private:
    QScrollArea* make_scrollable_container();
};

#endif // BOOKMARKFOLDER_H
