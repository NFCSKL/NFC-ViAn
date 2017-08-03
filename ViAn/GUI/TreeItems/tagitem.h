#ifndef TAGITEM_H
#define TAGITEM_H
#include "treeitem.h"
#include "Project/Analysis/tag.h"
class TagItem : public TreeItem {
    Tag* m_tag;
public:
    TagItem(Tag *tag);
    Tag *get_tag();
    void remove();
    void rename();
};

#endif // TAGITEM_H
