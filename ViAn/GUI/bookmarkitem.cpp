#include "bookmarkitem.h"

/**
 * @brief BookmarkItem::BookmarkItem
 * @param frame_nbr Frame number associated with the bookmark.
 * @param frame Frame image associated with the bookmark.
 * @param dir_path Path to the directory for the stored frame.
 * @param string Text description of the bookmark.
 * @param view Parent widget of the bookmark.
 */

BookmarkItem::BookmarkItem(int frame_nbr, QImage frame, QString dir_path, QString string, QListWidget* view) : QListWidgetItem(string, view) {
    bookmark = new Bookmark(frame_nbr, frame, dir_path, string);
    create_thumbnail(frame);
}

/**
 * @brief BookmarkItem::BookmarkItem
 * @param bookmrk Bookmark containing relevant.
 * @param view Parent widget of the bookmark.
 */
BookmarkItem::BookmarkItem(Bookmark* bookmrk, QListWidget* view) : QListWidgetItem(bookmrk->get_description(), view) {
    this->bookmark = bookmrk;
    QImage frame = bookmrk->get_frame();
    create_thumbnail(frame);
}

/**
 * @brief BookmarkItem::create_thumbnail
 * Creates and adds a thumbnail.
 * @param file_path Path to the stored image.
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
