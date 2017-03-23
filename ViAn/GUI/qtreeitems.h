#ifndef QTREEITEMS_H
#define QTREEITEMS_H

#include <QTreeWidgetItem>
#include <QObject>
#include <QWidget>
#include <string>

enum TYPE {PROJECT, VIDEO};


class QTreeProjectItem: public QTreeWidgetItem {
public:
    explicit QTreeProjectItem(int id, TYPE type);
    ~QTreeProjectItem();
    int id;
    TYPE type;
};
class QTreeVideoItem: public QTreeWidgetItem {
public:
    explicit QTreeVideoItem(int id);
    ~QTreeVideoItem();
    int id;
};

#endif // QTREEITEMS_H
