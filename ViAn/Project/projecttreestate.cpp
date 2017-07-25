#include "projecttreestate.h"

#include "Filehandler/saveable.h"
#include "utility.h"


ProjectTreeState::ProjectTreeState() {
}

void ProjectTreeState::read(const QJsonObject& json) {
    recursive_read(json, m_tree);
}

/**
 * @brief ProjectTreeState::write
 * @param json
 */
void ProjectTreeState::write(QJsonObject &json) {
    json = recursive_scan(m_tree);
}

/**
 * @brief ProjectTreeState::save_state
 * @param path
 */
void ProjectTreeState::save_state(const std::string& path) {
    save_saveable(path);
}

/**
 * @brief ProjectTreeState::load_state
 * @param path
 */
void ProjectTreeState::load_state(const std::string& path) {
    load_saveable(path);
}

/**
 * @brief ProjectTreeState::set_tree
 * @param p_wgt
 */
void ProjectTreeState::set_tree(QTreeWidgetItem *item) {
    m_tree = item;
}

/**
 * @brief ProjectTreeState::recursive_scan
 * Scans and stores the structure of the QTreeWidgetItem into a QJsonObject
 * @param item  :   QTreeWidgetItem to scan
 * @return folder   :   QJsonObject with tree structure
 */
QJsonObject ProjectTreeState::recursive_scan(QTreeWidgetItem *item) {
    QJsonObject folder;
    if (item->type() == FOLDER_ITEM) folder["name"] = item->text(0);
    for (auto i = 0; i < item->childCount(); ++i) {
        auto child = item->child(i);
        if (child->type() == FOLDER_ITEM) {
            folder[QString::fromStdString(Utility::zfill(std::to_string(i),
                                                         Utility::number_of_digits(item->childCount())))] = recursive_scan(child);
        } else {
            folder[QString::fromStdString(Utility::zfill(std::to_string(i),
                                                         Utility::number_of_digits(item->childCount())))] = child->type();
        }
    }
    return folder;
}

/**
 * @brief ProjectTreeState::recursive_read
 * Loads a tree structure from a QJsonObject and
 * recreates the items in the QTreeWidgetItem item
 * @param json  :   QJsonObject to read from
 * @param item  : QTreeWidgetItem to recreate items in
 */
void ProjectTreeState::recursive_read(const QJsonObject &json, QTreeWidgetItem* item) {
    for (auto it = json.begin(); it != json.end(); ++it) {
        if ((*it).isObject()) {
            // Folder
            FolderItem * f_item = new FolderItem();
            recursive_read((*it).toObject(), dynamic_cast<QTreeWidgetItem*>(f_item));
            item->addChild(f_item);
        } else if ((*it).isString()){
            item->setText(0, (*it).toString());
        } else {
            item->addChild(new VideoItem());
        }
    }
}
