#include "recentproject.h"

const std::string RecentProject::FILE_NAME = "recent_projects";
const std::string RecentProject::PATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation).toStdString();

RecentProject::RecentProject(){}

/**
 * @brief RecentProject::update_recent
 * Updates the recent projects file
 * @param name  :   project name
 * @param project_path  : path to the project
 */
void RecentProject::update_recent(const std::string& name, const std::string &project_path) {
    std::pair<std::string, std::string> new_proj = std::make_pair(name, project_path);
    auto item = std::find(recent_items.begin(), recent_items.end(), new_proj);
    if (item != recent_items.end()) recent_items.erase(item);
    recent_items.push_front(new_proj);
    QDir().mkpath(QString::fromStdString(PATH));
    save_saveable(PATH + "/" +FILE_NAME);
}

/**
 * @brief RecentProject::load_recent
 * Reads and returns recent projects
 * @return
 */
std::list<std::pair<std::string, std::string>> RecentProject::load_recent(){
    load_saveable(PATH + "/" + FILE_NAME);
    return recent_items;
}

/**
 * @brief RecentProject::read
 * @param json
 */
void RecentProject::read(const QJsonObject &json) {
    for (auto j : json["recent"].toArray()) {
        QJsonObject pair = j.toObject();
        QString key = pair.keys()[0];  //  Is always of length 1
        if(QFile(pair.value(key).toString()).exists())
            recent_items.push_back(std::make_pair(key.toStdString(), pair.value(key).toString().toStdString()));
    }
}

/**
 * @brief RecentProject::write
 * @param json
 */
void RecentProject::write(QJsonObject &json) {
    QJsonArray j_array;
    for (auto p : recent_items){
        QJsonObject pair;
        pair[QString::fromStdString(p.first)] = QString::fromStdString(p.second);
        j_array.append(pair);
    }
    json["recent"] = j_array;
}



