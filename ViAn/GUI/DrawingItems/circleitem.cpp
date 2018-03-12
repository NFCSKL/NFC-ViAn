#include "circleitem.h"

CircleItem::CircleItem(Circle* circle) : ShapeItem(CIRCLE_ITEM) {
    m_circle = circle;
    setText(0, QString::fromStdString(circle->get_name()));
}

void CircleItem::remove() {}

void CircleItem::rename() {
    m_circle->set_name(text(0).toStdString());
}

Circle* CircleItem::get_shape() {
    return m_circle;
}
