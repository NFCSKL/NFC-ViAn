#include "penitem.h"

PenItem::PenItem(Pen* pen) : ShapeItem(PEN_ITEM) {
    m_pen = pen;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, pen->get_name());
}

PenItem::~PenItem() {}

void PenItem::remove() {}

void PenItem::update_shape_name() {
    m_pen->set_name(text(0));
}
Pen* PenItem::get_shape() {
    return m_pen;
}
