#ifndef RECTITEM_H
#define RECTITEM_H

#include "GUI/TreeItems/treeitem.h"
#include "Video/shapes/rectangle.h"

class RectItem : public TreeItem {
    Rectangle* m_rect;
public:
    RectItem(Rectangle* rect);
    Rectangle* get_rect();
    void rename();
    void remove();
};

#endif // RECTITEM_H
