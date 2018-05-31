#include "bookmarkitem.h"
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>

/**
 * @brief BookmarkItem::BookmarkItem
 * @param bookmrk Bookmark containing relevant.
 * @param view Parent widget of the bookmark.
 */
QString BookmarkItem::get_description() const {
    return QString::fromStdString(bookmark->get_description());
}

BookmarkItem::BookmarkItem(Bookmark* bookmark,int type) : QListWidgetItem(QString::fromStdString(bookmark->get_description()), nullptr, type) {
    QString frame = QString::number(bookmark->get_frame_number());
    QString v_name = QString::fromStdString(bookmark->get_video_project()->get_video()->get_name());
    hover_text = "Source: " + v_name + "\nFrame: " + frame + "\nTime: " + bookmark->get_time();
    QString description = QString::fromStdString(bookmark->get_description());
    if (description != "") {
        setToolTip(hover_text + "\nDescription: " + description);
    } else {
        setToolTip(hover_text);
    }
    this->bookmark = bookmark;
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
void BookmarkItem::set_thumbnail(std::string thum_path) {
    QIcon icon(QString::fromStdString(thum_path));
    setIcon(icon);
}

/**
 * @brief BookmarkItem::copy
 * Returns a new BookmarkItem with the same bookmark pointer
 * @return
 */
BookmarkItem *BookmarkItem::copy() {
    BookmarkItem* new_bm_item = new BookmarkItem(bookmark, 0);
    new_bm_item->setIcon(icon());
    return new_bm_item;
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

QString BookmarkItem::get_file_path() {
    return QString::fromStdString(bookmark->m_file);
}

/**
 * @brief Bookmark::update_description
 * Sets the text description of the bookmark to the specified string,
 * and updates the text shown in the widget item.
 * @param text
 */
void BookmarkItem::update_description(const QString& text) {
    this->bookmark->set_description(text.toStdString());
    setText(text);
    if (text != "") {
        setToolTip(hover_text + "\nDescription: " + text);
    } else {
        setToolTip(hover_text);
    }
}
