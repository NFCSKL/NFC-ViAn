#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "GUI/TreeItems/treeitem.h"
#include "Video/shapes/circle.h"

class CircleItem : public TreeItem {
    Circle* m_circle;
public:
    CircleItem(Circle *circle);
    Circle* get_circle();
    void rename();
    void remove();
};

#endif // CIRCLEITEM_H
