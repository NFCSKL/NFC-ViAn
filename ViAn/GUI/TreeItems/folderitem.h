#ifndef FOLDERITEM_H
#define FOLDERITEM_H
#include "treeitem.h"
class FolderItem : public TreeItem {
public:
    FolderItem();
    ~FolderItem();
    void remove();
    void rename();
};

#endif // FOLDERITEM_H
