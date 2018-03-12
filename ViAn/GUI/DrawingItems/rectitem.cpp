#include "rectitem.h"

RectItem::RectItem(Rectangle* rect) : ShapeItem(RECT_ITEM) {
    m_rect = rect;
    setText(0, QString::fromStdString(rect->get_name()));
}

void RectItem::remove() {

}

void RectItem::rename() {
    m_rect->set_name(text(0).toStdString());
}

Rectangle* RectItem::get_shape() {
    return m_rect;
}
