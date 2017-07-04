#include "bookmarkcontainer.h"
#include <QDebug>
#include <QMimeData>
#include "GUI/bookmarkitem.h"
#include "bookmarklist.h"
#include <iostream>
/**
 * @brief BookmarkContainer::BookmarkContainer
 * @param parent
 */
BookmarkContainer::BookmarkContainer(QWidget *parent) : QListWidget(parent) {
    // Enable drag and drop
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    layout = new QVBoxLayout();
    setLayout(layout);
    setIconSize(QSize(80,80));


}

BookmarkContainer::~BookmarkContainer(){
    qDebug() << "Deleting container";
    delete layout;
}

void BookmarkContainer::dropEvent(QDropEvent *event) {
    const QMimeData* mime_data = event->mimeData();
    QByteArray encoded = mime_data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    int type;
    stream >> type;
    QListWidget* list = dynamic_cast<BookmarkList*>(event->source());
    if (type == 0) {
        BookmarkItem* bm_item = dynamic_cast<BookmarkItem*>(list->currentItem());
        // get container name
        addItem(bm_item->clone());

    }
}

