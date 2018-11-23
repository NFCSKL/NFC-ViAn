#include "recentproject.h"

#include "constants.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QStandardPaths>

RecentProject::RecentProject(){}

/**
 * @brief RecentProject::update_recent
 * Updates the recent projects file
 * @param name  :   project name
 * @param project_path  : path to the project
 */
void RecentProject::update_recent(const QString &name, const QString &project_path, const QString &last_changed) {
    std::tuple<QString, QString, QString> new_proj = std::make_tuple(name, project_path, last_changed);
    remove_project(project_path);
    if (recent_items.size() >= Constants::RECENT_MAX) recent_items.resize(Constants::RECENT_MAX);
    recent_items.push_front(new_proj);
    QDir().mkpath(Constants::RECENT_FILE_PATH);
    save();
}

/**
 * @brief RecentProject::remove_project
 * Remove a project from the recent_project list.
 * Returns true if successful else returns false;
 * @param name
 * @return
 */
bool RecentProject::remove_project(const QString& project_path) {
    load_recent();
    for (auto item : recent_items) {
        if (std::get<1>(item) == project_path) {
            recent_items.remove(item);
            save();
            return true;
        }
    }
    return false;
}

/**
 * @brief RecentProject::load_recent
 * Reads and returns recent projects
 * @return
 */
std::list<std::tuple<QString, QString, QString> > RecentProject::load_recent(){
    QString r_path = Constants::RECENT_FILE_PATH + Constants::RECENT_FILE_NAME;
    load_saveable(r_path);
    return recent_items;
}

/**
 * @brief RecentProject::save
 * Save the recent project file
 */
void RecentProject::save() {
    QString r_path = Constants::RECENT_FILE_PATH + Constants::RECENT_FILE_NAME;
    save_saveable(r_path);
}
/**
 * @brief RecentProject::read
 * @param json
 */
void RecentProject::read(const QJsonObject &json) {
    for (auto j : json["recent"].toArray()) {
        QJsonObject tuple = j.toObject();
        QString name = tuple["name"].toString();
        QString path = tuple["path"].toString();
        QString last_changed = tuple["last changed"].toString();
        if (QFile(path).exists()) {
            recent_items.push_back(std::make_tuple(name, path, last_changed));
        } else {
            qWarning("Failed to load file %s ", path.toStdString().c_str());
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
        tuple["name"] = std::get<0>(p);
        tuple["path"] = std::get<1>(p);
        tuple["last changed"] = std::get<2>(p);
        j_array.append(tuple);
    }
    json["recent"] = j_array;
}
