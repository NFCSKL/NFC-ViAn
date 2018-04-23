#ifndef DRAWINGTAGITEM_H
#define DRAWINGTAGITEM_H
#include "treeitem.h"
#include"Project/Analysis/drawingtag.h"

class DrawingTagItem : public TreeItem {
    DrawingTag* m_tag;

public:
    DrawingTagItem(DrawingTag *tag);
    DrawingTag *get_tag();
    void remove();
    void rename();
    //void update();
};

#endif // DRAWINGTAGITEM_H
