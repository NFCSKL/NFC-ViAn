#ifndef LISTITEM_H
#define LISTITEM_H
#include <QListWidgetItem>

enum DRAWING_ITEM {RECT = 2001, CIRCLE, LINE, ARROW, PEN, TEXT};
class ListItem : public QListWidgetItem {
public:
    ListItem(int type);
    virtual ~ListItem();
    virtual void remove() = 0;
    virtual void rename() = 0;
};

#endif // LISTITEM_H
