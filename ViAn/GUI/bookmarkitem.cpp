#include "bookmarkitem.h"

/**
 * @brief BookmarkItem::BookmarkItem
 * @param frame_nbr Frame number associated with the bookmark.
 * @param file_path Path to the stored image.
 * @param string Text description of the bookmark.
 * @param view Parent widget of the bookmark.
 */

BookmarkItem::BookmarkItem(int frame_nbr, QString file_path, QString string, QListWidget* view) : QListWidgetItem(string, view) {
    bookmark = new Bookmark(frame_nbr, file_path, string);
    create_thumbnail(file_path);
}

/**
 * @brief BookmarkItem::BookmarkItem
 * @param bookmrk Bookmark containing relevant.
 * @param view Parent widget of the bookmark.
 */
BookmarkItem::BookmarkItem(Bookmark bookmrk, QListWidget* view) : QListWidgetItem(bookmrk.get_description(), view) {
    bookmark = &bookmrk;
    create_thumbnail(bookmrk.get_file_path());
}

/**
 * @brief BookmarkItem::create_thumbnail
 * Creates and adds a thumbnail.
 * @param file_path Path to the stored image.
 */
void BookmarkItem::create_thumbnail(QString file_path) {
    QImage img = QImage(file_path, "TIFF");
    img = img.scaledToHeight(BOOKMARK_THUMBNAIL_HEIGHT);
    setData(Qt::DecorationRole, QPixmap::fromImage(img));
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
