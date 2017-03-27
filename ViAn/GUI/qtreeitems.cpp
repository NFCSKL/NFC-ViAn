#include "qtreeitems.h"

MyQTreeWidgetItem::MyQTreeWidgetItem(TYPE type, QString string, int id) {
    QTreeWidgetItem();
    this->id = id;
    this->type = type;
    this->name = string;
}

MyQTreeWidgetItem::~MyQTreeWidgetItem() {

}
