#include "bookmarkitem.h"

/**
 * @brief BookmarkItem::BookmarkItem
 * @param bookmrk Bookmark containing relevant.
 * @param view Parent widget of the bookmark.
 */
BookmarkItem::BookmarkItem(Bookmark* bookmark, QListWidget* view) : QListWidgetItem(bookmark->get_description(), view) {
    this->bookmark = bookmark;
    QImage frame = bookmark->get_frame();
    create_thumbnail(frame);
}

/**
 * @brief BookmarkItem::~BookmarkItem
 * Destructor, no memory allocated.
 */
BookmarkItem::~BookmarkItem() {
}

/**
 * @brief BookmarkItem::create_thumbnail
 * Creates and adds a thumbnail.
 * @param frame The image for the thumbnail.
 */
void BookmarkItem::create_thumbnail(QImage &frame) {
    frame = frame.scaledToHeight(BOOKMARK_THUMBNAIL_HEIGHT);
    setData(Qt::DecorationRole, QPixmap::fromImage(frame));
}

/**
 * @brief BookmarkItem::get_bookmark
 * @return Returns the bookmark representation.
 */
Bookmark* BookmarkItem::get_bookmark() {
    return bookmark;
}

/**
 * @brief BookmarkItem::get_frame_number
 * @return Returns the frame number that the bookmark points to.
 */
int BookmarkItem::get_frame_number() {
    return bookmark->get_frame_number();
}
