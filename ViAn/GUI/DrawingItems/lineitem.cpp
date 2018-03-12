#include "lineitem.h"

LineItem::LineItem(Line* line) : ShapeItem(LINE_ITEM) {
    m_line = line;
    setText(0, QString::fromStdString(line->get_name()));
}

void LineItem::remove() {}

void LineItem::rename() {
    m_line->set_name(text(0).toStdString());
}

Line* LineItem::get_shape() {
    return m_line;
}
