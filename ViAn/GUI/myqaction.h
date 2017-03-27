#ifndef MYQACTION_H
#define MYQACTION_H

#include <QObject>
#include <QAction>
#include "qtreeitems.h"

class MyQAction : public QAction
{
public:
    MyQAction();
    ~MyQAction();


signals:
    void triggered(MyQTreeWidgetItem *item, QAction *action);
};

#endif // MYQACTION_H
