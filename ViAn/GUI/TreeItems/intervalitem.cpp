#include "intervalitem.h"

#include "Project/Analysis/interval.h"

IntervalItem::IntervalItem(Interval *interval) : TreeItem(INTERVAL_ITEM) {
    m_interval = interval;
    setText(0, interval->get_name());
    const QIcon interval_icon("../ViAn/Icons/create_interval.png");
    setIcon(0, interval_icon);
}

IntervalItem::~IntervalItem() {}

void IntervalItem::remove() {}

void IntervalItem::rename() {}

Interval* IntervalItem::get_interval() {
    return m_interval;
}
