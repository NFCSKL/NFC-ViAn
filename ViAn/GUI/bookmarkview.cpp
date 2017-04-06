#include "bookmarkview.h"
#include <iostream>

/**
 * @brief BookmarkView::BookmarkView
 * Constructor.
 * @param parent Parent item of the BookmarkView.
 */
BookmarkView::BookmarkView(QListWidget* parent) {
    view = parent;
    view->addItem("Bookmarks:");
}

void BookmarkView::add_bookmark(std::string file_path) {
    QImage img = QImage(QString::fromStdString(file_path), "TIFF");
    img.scaledToHeight(BOOKMARK_HEIGHT);

    QListWidgetItem *item = new QListWidgetItem("", view);
    item->setData(Qt::DecorationRole, QPixmap::fromImage(img));

    view->addItem(item);

    //view->addItem(QString::fromStdString(file_path));
}
