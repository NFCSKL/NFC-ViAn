#include "bookmarkview.h"
#include "Library/customdialog.h"
#include <iostream>

/**
 * @brief BookmarkView::BookmarkView
 * Constructor.
 * @param parent Parent item of the BookmarkView.
 */
BookmarkView::BookmarkView(QListWidget* parent) {
    view = parent;
}

/**
 * @brief BookmarkView::get_num_bookmarks
 * @return Returns the number of bookmarks.
 */
int BookmarkView::get_num_bookmarks() {
    return view->count();
}

/**
 * @brief BookmarkView::add_bookmark
 * Adds a bookmark to the bookmark view.
 * @param video_id
 * @param bookmark_id
 * @param bookmark to add.
 */
void BookmarkView::add_bookmark(ID video_id, ID bookmark_id, Bookmark* bookmark) {
    BookmarkItem* bookmark_item = new BookmarkItem(bookmark, view);
    view->addItem(bookmark_item);
    this->bookmark_items[video_id][bookmark_id] = bookmark_item;
}

/**
 * @brief BookmarkView::remove_bookmark
 * Erases bookmarkitem from bookmarkview.
 * @param video_id
 * @param bookmark_id
 */
void BookmarkView::remove_bookmark(ID video_id, ID bookmark_id){
    BookmarkItem* bookmark_item = this->bookmark_items.at(video_id).at(bookmark_id);
    view->removeItemWidget(bookmark_item);
    this->bookmark_items.at(video_id).erase(bookmark_id);
    delete bookmark_item;
}

