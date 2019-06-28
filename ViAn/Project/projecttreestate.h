#ifndef PROJECTTREESTATE_H
#define PROJECTTREESTATE_H

#include "Filehandler/saveable.h"

#include <QJsonObject>

class QTreeWidgetItem;
class ProjectTreeState : public Saveable {
    QTreeWidgetItem* m_tree = nullptr;
public:
    ProjectTreeState();
    void load_tree();

    void read(const QJsonObject& json);
    void write(QJsonObject &json);

    void save_state(const QString &path);
    void load_state(const QString& path);

    void set_tree(QTreeWidgetItem *item);

private:
    QJsonObject recursive_scan(QTreeWidgetItem* top);
    void recursive_read(const QJsonObject& json, QTreeWidgetItem* item);
};

#endif // PROJECTTREESTATE_H
