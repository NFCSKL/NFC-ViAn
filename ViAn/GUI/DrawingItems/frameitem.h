#ifndef FRAMEITEM_H
#define FRAMEITEM_H

#include "GUI/TreeItems/treeitem.h"

class FrameItem : public TreeItem {
    int m_frame;
public:
    FrameItem(int frame);
    int get_frame();
    void remove();
    void rename();
};

#endif // FRAMEITEM_H
