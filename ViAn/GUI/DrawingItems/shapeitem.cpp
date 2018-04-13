#include "shapeitem.h"
#include <QDebug>

ShapeItem::ShapeItem(int type) : QTreeWidgetItem(type){
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

ShapeItem::~ShapeItem() {}

void ShapeItem::update_shape_name() {
    get_shape()->set_name(text(0));
}

void ShapeItem::update_shape_color() {
    map.fill(get_shape()->get_color());
    setIcon(1, map);
}
