#include "bookmarkbaseitem.h"

#include "Project/bookmark.h"
#include "bookmarkcategory.h"
#include "bookmarkitem.h"

BookmarkBaseItem::BookmarkBaseItem(QString description, int type)
    : QListWidgetItem(description, nullptr, type) {

}

bool BookmarkBaseItem::operator<(const QListWidgetItem& other) const {
    int index1, index2;
    if (this->type() == 1) {    // Type = container
        BookmarkCategory* c_item1 = dynamic_cast<BookmarkCategory*>(const_cast<BookmarkBaseItem*>(this));
        index1 = c_item1->get_index();
    } else {
        BookmarkItem* b_item1 = dynamic_cast<BookmarkItem*>(const_cast<BookmarkBaseItem*>(this));
        index1 = b_item1->get_bookmark()->get_index();
    }

    if (other.type() == 1) {    // Type = container
        BookmarkCategory* c_item1 = dynamic_cast<BookmarkCategory*>(const_cast<QListWidgetItem*>(&other));
        index2 = c_item1->get_index();
    } else {
        BookmarkItem* b_item1 = dynamic_cast<BookmarkItem*>(const_cast<QListWidgetItem*>(&other));
        index2 = b_item1->get_bookmark()->get_index();
    }

    bool res = index1 < index2;
    return res;
}
