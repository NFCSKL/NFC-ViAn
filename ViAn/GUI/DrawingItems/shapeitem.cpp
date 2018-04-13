#include "shapeitem.h"
#include <QDebug>

ShapeItem::ShapeItem(int type) : QTreeWidgetItem(type){
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

ShapeItem::~ShapeItem() {}
