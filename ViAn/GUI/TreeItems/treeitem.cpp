#include "treeitem.h"
#include <QDebug>

TreeItem::TreeItem(int type) : QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

TreeItem::~TreeItem() {
    qDebug() << "Treeitem delete";
}

