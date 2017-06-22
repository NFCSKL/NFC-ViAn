#include "bookmarkcontainer.h"
/**
 * @brief BookmarkContainer::BookmarkContainer
 * @param parent
 */
BookmarkContainer::BookmarkContainer(QWidget *parent) : QListWidget(parent)
{
    // Enable drag and drop
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    layout = new QVBoxLayout();
    setLayout(layout);
}
