#ifndef RECENTPROJECT_H
#define RECENTPROJECT_H

#include <QStandardPaths>
#include <QJsonArray>
#include <QDir>
#include <list>

#include "Filehandler/saveable.h"

class RecentProject : public Saveable {
    std::list<std::pair<std::string, std::string>> recent_items;
    static const std::string FILE_NAME;
    static const std::string PATH;
public:
    RecentProject();

    void update_recent(const std::string& name, const std::string& project_path);
    std::list<std::pair<std::string, std::string>> load_recent();

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};

#endif // RECENTPROJECT_H
