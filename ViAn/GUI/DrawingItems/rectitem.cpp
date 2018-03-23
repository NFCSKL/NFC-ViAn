#include "rectitem.h"
#include <QDebug>

RectItem::RectItem(Rectangle* rect) : ShapeItem(RECT_ITEM) {
    m_rect = rect;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, rect->get_name());
}

void RectItem::remove() {

}

void RectItem::rename() {
    m_rect->set_name(text(0));
}

Rectangle* RectItem::get_shape() {
    return m_rect;
}
