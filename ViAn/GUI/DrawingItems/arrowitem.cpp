#include "arrowitem.h"

ArrowItem::ArrowItem(Arrow* arrow) : ShapeItem(ARROW_ITEM) {
    m_arrow = arrow;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, arrow->get_name());
}

ArrowItem::~ArrowItem() {}

void ArrowItem::remove() {}

void ArrowItem::update_shape_name() {
    m_arrow->set_name(text(0));
}

Arrow* ArrowItem::get_shape() {
    return m_arrow;
}
