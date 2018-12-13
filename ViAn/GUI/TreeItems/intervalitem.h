#ifndef INTERVALITEM_H
#define INTERVALITEM_H

#include "treeitem.h"

class Interval;
class IntervalItem : public TreeItem
{
    Interval* m_interval;
public:
    IntervalItem(Interval* interval);
    ~IntervalItem();
    Interval* get_interval();
    void remove();
    void rename();
    //~Interval();
};

#endif // INTERVALITEM_H
