#include "bookmark.h"

/**
 * @brief Bookmark::Bookmark
 * @param img Thumbnail image.
 * @param string Text description of the bookmark.
 * @param view Parent widget of the bookmark.
 */
Bookmark::Bookmark(int frame_nbr, QImage img, QString string, QListWidget* view) : QListWidgetItem(string, view) {
    frame_number = frame_nbr;
    setData(Qt::DecorationRole, QPixmap::fromImage(img));
}

/**
 * @brief Bookmark::get_frame_number
 * @return Returns the frame number that the bookmark points to.
 */
int Bookmark::get_frame_number() {
    return frame_number;
}
