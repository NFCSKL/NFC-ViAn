#include "penitem.h"

PenItem::PenItem(Pen* pen) : ShapeItem(PEN_ITEM) {
    m_pen = pen;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, pen->get_name());
    const QIcon pen_icon("../ViAn/Icons/pen.png");
    setIcon(0, pen_icon);

    map = QPixmap(16,16);
    update_shape_color();
}

PenItem::~PenItem() {}

void PenItem::remove() {}


Pen* PenItem::get_shape() {
    return m_pen;
}
