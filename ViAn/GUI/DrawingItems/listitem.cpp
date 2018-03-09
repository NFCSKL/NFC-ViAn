#include "listitem.h"

ListItem::ListItem(int type) {
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

ListItem::~ListItem() {}
