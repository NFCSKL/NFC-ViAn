#include "arrowitem.h"

ArrowItem::ArrowItem(Arrow* arrow) : ShapeItem(ARROW_ITEM) {
    m_arrow = arrow;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, arrow->get_name());
    const QIcon arrow_icon("../ViAn/Icons/arrow.png");
    setIcon(0, arrow_icon);

    map = QPixmap(16,16);
    update_shape_color();
}

ArrowItem::~ArrowItem() {}

void ArrowItem::remove() {}

void ArrowItem::update_shape_name() {
    m_arrow->set_name(text(0));
}

void ArrowItem::update_shape_color() {
    map.fill(m_arrow->get_color());
    setIcon(1, map);
}

Arrow* ArrowItem::get_shape() {
    return m_arrow;
}
