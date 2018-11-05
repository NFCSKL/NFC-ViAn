#include "intervalitem.h"

#include "Project/Analysis/interval.h"

IntervalItem::IntervalItem(Interval *interval) : TreeItem(INTERVAL_ITEM) {
    m_interval = interval;
    setText(0, QString::fromStdString(interval->get_name()));

}

IntervalItem::~IntervalItem() {}

void IntervalItem::remove() {}

void IntervalItem::rename() {}

