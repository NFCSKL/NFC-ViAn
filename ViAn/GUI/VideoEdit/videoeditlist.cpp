#include "videoeditlist.h"

VideoEditList::VideoEditList()
{
    for(int i = 1; i < 10; ++i)
        addItem(new QListWidgetItem(QString("Item %1").arg(i)));
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setFlow(QListView::TopToBottom);
//    setFlow(QListView::LeftToRight);

}
