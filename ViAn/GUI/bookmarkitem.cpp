#include "bookmarkitem.h"

/**
 * @brief BookmarkItem::BookmarkItem
 * @param frame_nbr Frame number associated with the bookmark.
 * @param img Thumbnail image.
 * @param string Text description of the bookmark.
 * @param view Parent widget of the bookmark.
 */
BookmarkItem::BookmarkItem(int frame_nbr, std::string file_path, QString string, QListWidget* view) : QListWidgetItem(string, view) {
    bookmark = new Bookmark(frame_nbr, file_path, string);
    QImage img = QImage(QString::fromStdString(file_path), "TIFF");
    img = img.scaledToHeight(BOOKMARK_THUMBNAIL_HEIGHT);
    setData(Qt::DecorationRole, QPixmap::fromImage(img));
}

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
