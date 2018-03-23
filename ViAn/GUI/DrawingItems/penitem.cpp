#include "penitem.h"

PenItem::PenItem(Pen* pen) : ShapeItem(PEN_ITEM) {
    m_pen = pen;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, QString::fromStdString(pen->get_name()));
}

void PenItem::remove() {}

void PenItem::rename() {
    m_pen->set_name(text(0).toStdString());
}
Pen* PenItem::get_shape() {
    return m_pen;
}
