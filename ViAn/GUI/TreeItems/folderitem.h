#ifndef FOLDERITEM_H
#define FOLDERITEM_H
#include "treeitem.h"
class FolderItem : public TreeItem {
public:
    FolderItem();
    void remove();
    void rename();
};

#endif // FOLDERITEM_H
