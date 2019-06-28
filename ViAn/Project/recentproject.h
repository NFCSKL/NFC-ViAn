#ifndef RECENTPROJECT_H
#define RECENTPROJECT_H

#include "Filehandler/saveable.h"

#include <QJsonObject>

#include <list>
#include <tuple>

class RecentProject : public Saveable {
    std::list<std::tuple<QString, QString, QString>> recent_items;

public:
    RecentProject();

    void update_recent(const QString& name, const QString& project_path, const QString& last_changed);
    bool remove_project(const QString &project_path);
    std::list<std::tuple<QString, QString, QString>> load_recent();
    void save();

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};

#endif // RECENTPROJECT_H
