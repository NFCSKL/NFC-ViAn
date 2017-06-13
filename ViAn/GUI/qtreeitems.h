#ifndef QTREEITEMS_H
#define QTREEITEMS_H

#include <QTreeWidgetItem>
#include <QObject>
#include <QWidget>
#include <string>
#include <vector>
#include "bookmarkview.h"
#define TEXT_LENGTH 15

enum TYPE {PROJECT, VIDEO, ANALYSIS};

typedef int ID;

class MyQTreeWidgetItem: public QTreeWidgetItem {
public:
    MyQTreeWidgetItem(TYPE type, QString name = "", ID id = -1);
    ~MyQTreeWidgetItem();
    ID id;
    TYPE type;
    QString name;
    void set_text(QString text);
    void set_text_from_filepath(std::string filepath);
    std::string get_name();
    bool operator==(MyQTreeWidgetItem* item);
private slots:

private:


};

class QTreeVideoItem :public MyQTreeWidgetItem{
public:
    explicit QTreeVideoItem(TYPE type, QString name = "", ID id = -1);
    std::vector<ID> bookmarks;
    BookmarkView *bookmark_view;
};

#endif // QTREEITEMS_H
