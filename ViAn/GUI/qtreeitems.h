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
    void set_text(std::string text);
    void set_text_from_filepath(std::string filepath);
private slots:

private:


};

#endif // QTREEITEMS_H
