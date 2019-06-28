#include "shapeitem.h"
#include "Video/shapes/shapes.h"
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

void ShapeItem::update_show_icon(bool show) {
    if (show) {
        const QIcon show_icon(":/Icons/show.png");
        setIcon(2, show_icon);
    } else {
        const QIcon hide_icon(":/Icons/hide.png");
        setIcon(2, hide_icon);
    }
}
