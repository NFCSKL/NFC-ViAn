#ifndef PROJECTTREESTATE_H
#define PROJECTTREESTATE_H
#include "Filehandler/saveable.h"
#include "Filehandler/saveable.h"
#include "GUI/projectwidget.h"
#include "GUI/TreeItems/folderitem.h"
#include "GUI/TreeItems/videoitem.h"
class ProjectWidget;
class ProjectTreeState : public Saveable {
    QTreeWidgetItem* m_tree = nullptr;
public:
    ProjectTreeState();
    void load_tree();

    void read(const QJsonObject& json);
    void write(QJsonObject &json);

    void save_state(const std::string& path);
    void load_state(const std::string& path);

    void set_tree(QTreeWidgetItem *item);

private:
    QJsonObject recursive_scan(QTreeWidgetItem* top);
    void recursive_read(const QJsonObject& json, QTreeWidgetItem* item);
};

#endif // PROJECTTREESTATE_H
