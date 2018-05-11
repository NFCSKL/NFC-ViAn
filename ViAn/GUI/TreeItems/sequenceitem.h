#ifndef SEQUENCEITEM_H
#define SEQUENCEITEM_H

#include "treeitem.h"
#include "Project/videoproject.h"
class SequenceItem : public TreeItem {
private:
    int m_index{};
public:
    SequenceItem(const std::string& name, const int& index);
    int get_index();
    void remove();
    void rename();
};

#endif // SEQUENCEITEM_H
