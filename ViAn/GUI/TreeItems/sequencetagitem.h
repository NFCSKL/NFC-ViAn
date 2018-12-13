#ifndef SEQUENCETAGITEM_H
#define SEQUENCETAGITEM_H

#include "Project/video.h"
#include "treeitem.h"

class TagFrame;
class SequenceTagItem : public TreeItem {
    int m_index{};
    TagFrame* m_tag;
public:
    SequenceTagItem(const QString& name, const int& index, TagFrame *tag);
    int get_index();
    VideoState get_state();
    void remove();
    void rename();
};

#endif // SEQUENCETAGITEM_H
