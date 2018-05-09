#ifndef TAGFRAMEITEM_H
#define TAGFRAMEITEM_H
#include "treeitem.h"

class TagFrameItem : public TreeItem {
    int m_frame;
public:
    TagFrameItem(int frame);
    int get_frame();
    void remove();
    void rename();
};

#endif // TAGFRAMEITEM_H
