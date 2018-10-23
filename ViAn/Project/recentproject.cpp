#include "recentproject.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>

const std::string RecentProject::FILE_NAME = "recent_projects";
const std::string RecentProject::PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation).toStdString()+ "/ViAn/";

RecentProject::RecentProject(){}

/**
 * @brief RecentProject::update_recent
 * Updates the recent projects file
 * @param name  :   project name
 * @param project_path  : path to the project
 */
void RecentProject::update_recent(const std::string& name, const std::string &project_path, const std::string &last_changed) {
    std::tuple<std::string, std::string, std::string> new_proj = std::make_tuple(name, project_path, last_changed);

    for (auto item : recent_items) {
        if (std::get<1>(item) == std::get<1>(new_proj)) {
            recent_items.remove(item);
        }
    }
    if (recent_items.size() >= RECENT_MAX) recent_items.resize(RECENT_MAX);
    recent_items.push_front(new_proj);
    QDir().mkpath(QString::fromStdString(PATH));
    save_saveable(PATH + FILE_NAME);
}

/**
 * @brief RecentProject::load_recent
 * Reads and returns recent projects
 * @return
 */
std::list<std::tuple<std::string, std::string, std::string> > RecentProject::load_recent(){
    load_saveable(PATH + FILE_NAME);
    return recent_items;
}

/**
 * @brief RecentProject::read
 * @param json
 */
void RecentProject::read(const QJsonObject &json) {
    for (auto j : json["recent"].toArray()) {
        QJsonObject tuple = j.toObject();
        std::string name = tuple["name"].toString().toStdString();
        std::string path = tuple["path"].toString().toStdString();
        std::string last_changed = tuple["last changed"].toString().toStdString();
        if (QFile(QString::fromStdString(path)).exists()) {
            recent_items.push_back(std::make_tuple(name, path, last_changed));
        }
    }
}

/**
 * @brief RecentProject::write
 * @param json
 */
void RecentProject::write(QJsonObject &json) {
    QJsonArray j_array;
    for (auto p : recent_items){
        QJsonObject tuple;
        tuple["name"] = QString::fromStdString(std::get<0>(p));
        tuple["path"] = QString::fromStdString(std::get<1>(p));
        tuple["last changed"] = QString::fromStdString(std::get<2>(p));
        j_array.append(tuple);
    }
    json["recent"] = j_array;
}
