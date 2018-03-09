#include "rectitem.h"

RectItem::RectItem(Rectangle* rect) : TreeItem(RECT_ITEM) {
    m_rect = rect;
}

void RectItem::remove() {

}

void RectItem::rename() {

}

Rectangle* RectItem::get_rect() {
    return m_rect;
}
