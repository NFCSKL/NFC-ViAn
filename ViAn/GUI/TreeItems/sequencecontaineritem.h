#ifndef SEQUENCECONTAINERITEM_H
#define SEQUENCECONTAINERITEM_H

#include "treeitem.h"

class SequenceContainerItem : public TreeItem {
private:
    const QString BASE_NAME = "Images";
public:
    SequenceContainerItem();
    void update_text();
    void remove();
    void rename();
};

#endif // SEQUENCECONTAINERITEM_H
