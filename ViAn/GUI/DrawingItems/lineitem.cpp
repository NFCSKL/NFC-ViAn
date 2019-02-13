#include "lineitem.h"

LineItem::LineItem(Line* line) : ShapeItem(LINE_ITEM) {
    m_line = line;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, line->get_name());
    const QIcon line_icon(":/Icons/line.png");
    setIcon(0, line_icon);

    map = QPixmap(16,16);
    update_shape_color();
    update_show_icon(m_line->get_show());
}

LineItem::~LineItem() {}

void LineItem::remove() {}

Line* LineItem::get_shape() {
    return m_line;
}
