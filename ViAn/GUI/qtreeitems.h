#ifndef QTREEITEMS_H
#define QTREEITEMS_H

#include <QTreeWidgetItem>
#include <QObject>
#include <QWidget>
#include <string>

enum TYPE {PROJECT, VIDEO};


class MyQTreeWidgetItem: public QTreeWidgetItem {
public:
    explicit MyQTreeWidgetItem(TYPE type, QString name = "", int id = 0);
    ~MyQTreeWidgetItem();
    int id;
    TYPE type;
    QString name;
private slots:

private:


};

#endif // QTREEITEMS_H
