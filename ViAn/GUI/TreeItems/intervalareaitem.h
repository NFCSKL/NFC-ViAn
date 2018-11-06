#ifndef INTERVALAREA_H
#define INTERVALAREA_H

#include "Project/video.h"
#include "treeitem.h"

class IntervalAreaItem : public TreeItem {

    int m_start = -1;
    int m_end = -1;

public:
    IntervalAreaItem(int start, int end);
    int get_start();
    int get_end();
    void remove();
    void rename();
};

#endif // INTERVALAREA_H
