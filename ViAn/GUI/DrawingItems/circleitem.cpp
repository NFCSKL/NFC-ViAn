#include "circleitem.h"

CircleItem::CircleItem(Circle* circle) : TreeItem(CIRCLE_ITEM) {
    m_circle = circle;
}

void CircleItem::remove() {}

void CircleItem::rename() {}

Circle* CircleItem::get_circle() {
    return m_circle;
}
