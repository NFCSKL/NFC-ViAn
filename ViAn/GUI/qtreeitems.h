#ifndef QTREEITEMS_H
#define QTREEITEMS_H

#include <QTreeWidgetItem>
#include <QObject>
#include <QWidget>
#include <string>
#define TEXT_LENGTH 15

enum TYPE {PROJECT, VIDEO, ANALYSIS};


class MyQTreeWidgetItem: public QTreeWidgetItem {
public:
    explicit MyQTreeWidgetItem(TYPE type, QString name = "", int id = 0);
    ~MyQTreeWidgetItem();
    int id;
    TYPE type;
    QString name;
    void set_text(std::string text);
    void set_text_from_filepath(std::string filepath);
    std::string get_name();
    bool operator==(MyQTreeWidgetItem* item);
private slots:

private:


};

#endif // QTREEITEMS_H
