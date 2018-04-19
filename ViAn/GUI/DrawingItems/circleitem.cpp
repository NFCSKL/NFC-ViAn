#include "circleitem.h"
#include <QDebug>

CircleItem::CircleItem(Circle* circle) : ShapeItem(CIRCLE_ITEM) {
    m_circle = circle;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, circle->get_name());
    const QIcon circle_icon("../ViAn/Icons/circle.png");
    setIcon(0, circle_icon);

    map = QPixmap(16,16);
    update_shape_color();
}

CircleItem::~CircleItem() {}

void CircleItem::remove() {}

void CircleItem::update_shape_name() {
    m_circle->set_name(text(0));
}

void CircleItem::update_shape_color() {
    map.fill(m_circle->get_color());
    setIcon(1, map);
}

Circle* CircleItem::get_shape() {
    return m_circle;
}
