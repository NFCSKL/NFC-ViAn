#ifndef SEQUENCETAGITEM_H
#define SEQUENCETAGITEM_H

#include "treeitem.h"
#include "Project/Analysis/tagframe.h"

class SequenceTagItem : public TreeItem {
private:
    int m_index{};
    TagFrame* m_tag;
public:
    SequenceTagItem(const std::string& name, const int& index, TagFrame *tag);
    int get_index();
    void remove();
    void rename();
};

#endif // SEQUENCETAGITEM_H
