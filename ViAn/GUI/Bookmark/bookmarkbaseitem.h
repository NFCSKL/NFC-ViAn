#ifndef BOOKMARKBASEITEM_H
#define BOOKMARKBASEITEM_H

#include <QListWidgetItem>

class BookmarkBaseItem : public QListWidgetItem {

public:
    BookmarkBaseItem(QString description, int type);
    bool operator<(const QListWidgetItem &other) const;
};

#endif // BOOKMARKBASEITEM_H
