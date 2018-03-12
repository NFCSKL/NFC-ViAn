#include "shapeitem.h"

ShapeItem::ShapeItem(int type) : QTreeWidgetItem(type){
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

ShapeItem::~ShapeItem() {}
