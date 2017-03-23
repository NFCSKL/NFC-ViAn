#include "qtreeitems.h"

QTreeProjectItem::QTreeProjectItem(int id, TYPE type) {
    QTreeWidgetItem();
    this->id = id;
    this->type = type;
}

QTreeProjectItem::~QTreeProjectItem() {

}

QTreeVideoItem::QTreeVideoItem(int id) {
    QTreeWidgetItem();
    this->id = id;
}

QTreeVideoItem::~QTreeVideoItem() {

}
