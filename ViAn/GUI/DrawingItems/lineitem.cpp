#include "lineitem.h"

LineItem::LineItem(Line* line) : ShapeItem(LINE_ITEM) {
    m_line = line;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, line->get_name());
}

void LineItem::remove() {}

void LineItem::rename() {
    m_line->set_name(text(0));
}

Line* LineItem::get_shape() {
    return m_line;
}
