#ifndef BOOKMARKVIEW_H
#define BOOKMARKVIEW_H

#include "bookmarkitem.h"
#include <QListWidget>
#include <map>
class BookmarkView {
public:
    BookmarkView(QListWidget* parent);    
    int add_bookmark(ID video_id, ID bookmark_id, Bookmark *bookmark);
    void remove_bookmark(ID video_id, ID bookmark_id);
    int get_num_bookmarks();
    QString get_input_text(bool* ok);
private:

    std::map<ID, std::map<ID, BookmarkItem*>> bookmark_items;
    const int TEXT_EDIT_MIN_HEIGHT = 32;
    QListWidget* view;
};

#endif // BOOKMARKVIEW_H
