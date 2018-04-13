#include "circleitem.h"
#include <QDebug>

CircleItem::CircleItem(Circle* circle) : ShapeItem(CIRCLE_ITEM) {
    m_circle = circle;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, circle->get_name());
}

CircleItem::~CircleItem() {}

void CircleItem::remove() {}

void CircleItem::update_shape_name() {
    m_circle->set_name(text(0));
}

Circle* CircleItem::get_shape() {
    return m_circle;
}
