#include "arrowitem.h"

ArrowItem::ArrowItem(Arrow* arrow) : ShapeItem(ARROW_ITEM) {
    m_arrow = arrow;
    setText(0, QString::fromStdString((arrow->get_name())));
}

void ArrowItem::remove() {}

void ArrowItem::rename() {
    m_arrow->set_name(text(0).toStdString());
}

Arrow* ArrowItem::get_shape() {
    return m_arrow;
}
