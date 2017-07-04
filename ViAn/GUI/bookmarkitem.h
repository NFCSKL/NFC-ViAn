#ifndef BOOKMARKITEM_H
#define BOOKMARKITEM_H
#include "Project/bookmark.h"
#include <QListWidgetItem>

class BookmarkItem : public QListWidgetItem {
public:
    BookmarkItem(Bookmark *bookmark, int type = 1);
    ~BookmarkItem();
    Bookmark* get_bookmark();
    int get_frame_number();
    void update_description(const QString &text);
    void set_thumbnail(std::string thum_path);
private:
    const int BOOKMARK_THUMBNAIL_HEIGHT = 64;
    Bookmark* bookmark;
protected:
//    void mousePressEvent(QMouseEvent* event) override;
};

#endif // BOOKMARKITEM_H
