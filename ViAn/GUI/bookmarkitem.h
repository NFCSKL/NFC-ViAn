#ifndef BOOKMARKITEM_H
#define BOOKMARKITEM_H
#include "Filehandler/Project/bookmark.h"
#include <QListWidgetItem>

class BookmarkItem : public QListWidgetItem {
public:
    BookmarkItem(Bookmark *bookmark, QListWidget *view);
    ~BookmarkItem();
    Bookmark* get_bookmark();
    int get_frame_number();
    void update_description(QString text);
private:
    void create_thumbnail(QImage &frame);
    const int BOOKMARK_THUMBNAIL_HEIGHT = 64;
    Bookmark* bookmark;
};

#endif // BOOKMARKITEM_H
