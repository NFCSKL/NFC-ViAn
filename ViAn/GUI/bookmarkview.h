#ifndef BOOKMARKVIEW_H
#define BOOKMARKVIEW_H

#include "bookmarkitem.h"
#include <QListWidget>
#include <map>
class BookmarkView {
public:
    BookmarkView(QListWidget* parent);    
    int add_bookmark(Bookmark *bookmark);
    int get_num_bookmarks();
    QString get_input_text(bool* ok);
private:

    std::map<int, BookmarkItem*> boomark_items;
    const int TEXT_EDIT_MIN_HEIGHT = 32;
    int id_bookmark = 0;
    QListWidget* view;
};

#endif // BOOKMARKVIEW_H
