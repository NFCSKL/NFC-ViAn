#ifndef RECENTPROJECT_H
#define RECENTPROJECT_H

#include "Filehandler/saveable.h"

#include <QJsonObject>

#include <list>
#include <tuple>

class RecentProject : public Saveable {
    std::list<std::tuple<std::string, std::string, std::string>> recent_items;

public:
    RecentProject();

    void update_recent(const std::string& name, const std::string& project_path, const std::string& last_changed);
    bool remove_project(const std::string& project_path);
    std::list<std::tuple<std::string, std::string, std::string>> load_recent();
    void save();

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};

#endif // RECENTPROJECT_H
