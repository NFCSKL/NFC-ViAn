#include "bookmarkitem.h"

#include "Project/bookmark.h"
#include "Project/videoproject.h"

#include <QDebug>

/**
 * @brief BookmarkItem::BookmarkItem
 * @param bookmark Bookmark to be saved in the item.
 * @param type Type of bookmark item.
 */
BookmarkItem::BookmarkItem(Bookmark* bookmark, int type) : QListWidgetItem(bookmark->get_description(), nullptr, type) {
    QString frame = QString::number(bookmark->get_frame_number());
    hover_text = "Source: " + bookmark->m_image_name + "\nFrame: " + frame +
            "\nTime: " + bookmark->get_time() + "\nIndex: " + QString::number(bookmark->get_index());
    QString description = bookmark->get_description();
    if (description != "") {
        setToolTip(hover_text + "\nDescription: " + description);
    } else {
        setToolTip(hover_text);
    }
    m_bookmark = bookmark;
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
void BookmarkItem::set_thumbnail(QString thum_path) {
    QIcon icon(thum_path);
    setIcon(icon);
}

/**
 * @brief BookmarkItem::copy
 * Returns a new BookmarkItem with the same bookmark pointer
 * @return
 */
BookmarkItem *BookmarkItem::copy() {
    BookmarkItem* new_bm_item = new BookmarkItem(m_bookmark, 0);
    new_bm_item->setIcon(icon());
    return new_bm_item;
}

/**
 * @brief BookmarkItem::get_bookmark
 * @return Returns the bookmark representation.
 */
Bookmark* BookmarkItem::get_bookmark() {
    return m_bookmark;
}

/**
 * @brief BookmarkItem::get_frame_number
 * @return Returns the frame number that the bookmark points to.
 */
int BookmarkItem::get_frame_number() {
    return m_bookmark->get_frame_number();
}

QString BookmarkItem::get_file_path() {
    return m_bookmark->m_file;
}

QString BookmarkItem::get_description() const {
    return m_bookmark->get_description();
}

QString BookmarkItem::get_time() const {
    return m_bookmark->get_time();
}

/**
 * @brief Bookmark::update_description
 * Sets the text description of the bookmark to the specified string,
 * and updates the text shown in the widget item.
 * @param text
 */
void BookmarkItem::update_description(const QString& text) {
    m_bookmark->set_description(text);
    setText(text);
    if (text != "") {
        setToolTip(hover_text + "\nDescription: " + text);
    } else {
        setToolTip(hover_text);
    }
}

bool QListWidgetItem::operator<(const QListWidgetItem& other) {
    BookmarkItem* b_item = dynamic_cast<BookmarkItem*>(const_cast<QListWidgetItem*>(&other));
    qDebug() << this->m_bookmark->get_index() << b_item->get_bookmark()->get_index();
    bool res = this->m_bookmark->get_index() < b_item->get_bookmark()->get_index();
    qDebug() << "result" << res;
    return res;
}
