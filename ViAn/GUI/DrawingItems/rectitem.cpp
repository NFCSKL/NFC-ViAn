#include "rectitem.h"
#include <QDebug>

RectItem::RectItem(Rectangle* rect) : ShapeItem(RECT_ITEM) {
    m_rect = rect;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, rect->get_name());
    const QIcon rect_icon("../ViAn/Icons/box.png");
    setIcon(0, rect_icon);

    map = QPixmap(16,16);
    update_shape_color();
}

RectItem::~RectItem() {}

void RectItem::remove() {}

void RectItem::update_shape_name() {
    m_rect->set_name(text(0));
}

void RectItem::update_shape_color() {
    map.fill(m_rect->get_color());
    setIcon(1, map);
}

Rectangle* RectItem::get_shape() {
    return m_rect;
}