#ifndef TREEITEM_H
#define TREEITEM_H
#include <QTreeWidgetItem>
// ENUM needs to start at 1001, otherwise conflict with QTreeWidgetItems internal types
enum ITEM_TYPE {VIDEO_ITEM=1001, ANALYSIS_ITEM, FOLDER_ITEM, TAG_ITEM};
class TreeItem : public QTreeWidgetItem {
public:
    TreeItem(int type);
    virtual ~TreeItem();
    virtual void remove() = 0;
    virtual void rename() = 0;
};

#endif // TREEITEM_H
