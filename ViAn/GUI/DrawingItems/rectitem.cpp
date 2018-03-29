#include "rectitem.h"
#include <QDebug>

RectItem::RectItem(Rectangle* rect) : ShapeItem(RECT_ITEM) {
    m_rect = rect;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, rect->get_name());
}

RectItem::~RectItem() {
    delete m_rect;
}

void RectItem::remove() {}

void RectItem::update_shape_name() {
    m_rect->set_name(text(0));
}

Rectangle* RectItem::get_shape() {
    return m_rect;
}
