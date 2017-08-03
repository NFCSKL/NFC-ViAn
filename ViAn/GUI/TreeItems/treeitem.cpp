#include "treeitem.h"

TreeItem::TreeItem(int type) : QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

