#include "intervalareaitem.h"

#include "intervalitem.h"

IntervalAreaItem::IntervalAreaItem(int start, int end) : TreeItem(INTERVAL_AREA_ITEM) {
    m_start = start;
    m_end = end;
    QString name = QString::number(m_start) + " - " + QString::number(m_end);
    setText(0, name);
    setFlags(flags() & ~Qt::ItemIsEditable);
}

int IntervalAreaItem::get_start() {
    return m_start;
}

int IntervalAreaItem::get_end() {
    return m_end;
}

bool IntervalAreaItem::is_in_interval(int frame) {
    return frame >= m_start && frame <= m_end;
}

void IntervalAreaItem::remove() {}

void IntervalAreaItem::rename() {}
