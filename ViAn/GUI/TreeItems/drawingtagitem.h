#ifndef DRAWINGTAGITEM_H
#define DRAWINGTAGITEM_H

#include "treeitem.h"

class Tag;
class DrawingTagItem : public TreeItem {
    Tag* m_tag;

public:
    DrawingTagItem(Tag *tag);
    ~DrawingTagItem();
    Tag *get_tag();
    void remove();
    void rename();
    //void update();
};

#endif // DRAWINGTAGITEM_H
