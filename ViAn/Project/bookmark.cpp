#include "bookmark.h"

#include "project.h"
#include "utility.h"
#include "videoproject.h"

#include <QDebug>

/**
 * @brief Bookmark::Bookmark
 * @param file_name file name for bookmark
 * @param text Text description of the bookmark.
 * @param state State of the video containing info like frame number and zoom level.
 * @param time The time in the video associated with the bookmark (format "mm:ss").
 * @param type The type of the bookmark, which list it is in.
 */
Bookmark::Bookmark(const QString &file_name, const QString &text,
                   const VideoState &state, const QString &time,
                   const int &type){
    m_file = file_name;
    m_description = text;
    m_state = state;
    m_time = time;
    m_type = type;
}

/**
 * @brief Bookmark::Bookmark
 * Copy constructor
 * @param bookmark
 */
Bookmark::Bookmark(const Bookmark &bookmark) {
    m_proj = bookmark.m_proj;
    m_vid_proj_id = bookmark.m_vid_proj_id;
    m_time = bookmark.m_time;
    m_description = bookmark.m_description;
    m_state = bookmark.m_state;
    m_thumbnail_path = bookmark.m_thumbnail_path;
    m_index = bookmark.m_index;
    m_type = bookmark.m_type;
    m_container_id = bookmark.m_container_id;

    m_file = bookmark.m_file;
    m_image_name = bookmark.m_image_name;
}

/**
 * @brief Bookmark::Bookmark
 * Null initializing constructor.
 */
Bookmark::Bookmark() {
}

/**
 * @brief Bookmark::~Bookmark
 * Destructor
 */
Bookmark::~Bookmark(){
}

int Bookmark::get_vid_proj_id() {
    return m_vid_proj_id;
}

void Bookmark::set_vid_proj_id(int new_id) {
    m_vid_proj_id = new_id;
}

int Bookmark::get_index() {
    return m_index;
}

void Bookmark::set_index(int index) {
    m_index = index;
}

int Bookmark::get_type() {
    return m_type;
}

void Bookmark::reset_root_dir(const QString &dir) {
    m_file = dir + Constants::BOOKMARK_FOLDER + Utility::name_from_path(m_file);
    m_thumbnail_path = dir + Constants::THUMBNAIL_FOLDER + Utility::name_from_path(m_thumbnail_path);
    m_unsaved_changes = true;
}

/**
 * @brief Bookmark::get_time
 * @return Returns the time in the video where the bookmark points to (format "mm:ss").
 */
QString Bookmark::get_time() {
    return m_time;
}

/**
 * @brief Bookmark::get_frame_number
 * @return Returns the frame number that the bookmark points to.
 */
int Bookmark::get_frame_number() {
    return m_state.frame;
}

VideoState Bookmark::get_state() {
    return m_state;
}

int Bookmark::get_container_id() {
    return m_container_id;
}

Project* Bookmark::get_project() {
    return m_proj;
}

/**
 * @brief Bookmark::get_description
 * @return Returns the description associated with the bookmark.
 */
QString Bookmark::get_description() {
    return m_description;
}

/**
 * @brief Bookmark::set_description
 * Sets the text description of the bookmark to the specified string.
 * @param text
 */
void Bookmark::set_description(const QString& text) {
    m_description = text;
    m_unsaved_changes = true;
}

void Bookmark::set_container(int new_id, int type) {
    m_container_id = new_id;
    m_type = type;
    m_unsaved_changes = true;
}

/**
 * @brief Bookmark::set_thumbnail_path
 * Saves the path where the thumbnail for the bookmark is saved.
 * @param path
 */
void Bookmark::set_thumbnail_path(QString path) {
    m_thumbnail_path = path;
    m_unsaved_changes = true;
}

QString Bookmark::get_thumbnail_path() {
    return m_thumbnail_path;
}

void Bookmark::set_project(Project* proj) {
    m_proj = proj;
    m_unsaved_changes = true;
}

/**
 * @brief Bookmark::read
 * @param json
 * Reads a bookmark from a Json object.
 */
void Bookmark::read(const QJsonObject& json){
    m_vid_proj_id = json["vid_proj id"].toInt();
    m_time = json["time"].toString();
    m_file = json["path"].toString();
    m_thumbnail_path = json["thumbnail path"].toString();
    m_description = json["description"].toString();
    m_index = json["index"].toInt();
    m_type = json["type"].toInt();
    m_container_id = json["container"].toInt();
    m_image_name = json["image name"].toString();

    VideoState state;
    state.frame = json["frame"].toInt();
    state.scale_factor = json["scale_factor"].toDouble();
    int x = json["anchor x"].toInt();
    int y = json["anchor y"].toInt();
    state.anchor = QPoint(x, y);
    x = json["center_x"].toInt();
    y = json["center_y"].toInt();
    state.center = QPoint(x, y);
    state.rotation = json["rotation"].toInt();
    state.brightness = json["brightness"].toInt();
    state.contrast = json["contrast"].toDouble();
    state.gamma = json["gamma"].toDouble();
    m_state = state;
    
    m_unsaved_changes = false;
}

/**
 * @brief Bookmark::write
 * @param json
 * Writes a bookmark to a Json object.
 */
void Bookmark::write(QJsonObject& json){
    json["vid_proj id"] = m_vid_proj_id;
    json["time"] = m_time;
    json["path"] = m_file;
    json["thumbnail path"] = m_thumbnail_path;
    json["description"] = m_description;
    json["index"] = m_index;
    json["container"] = m_container_id;
    json["type"] = m_type;
    json["image name"] = m_image_name;

    json["frame"] = m_state.frame;
    json["scale_factor"] = m_state.scale_factor;
    json["anchor x"] = m_state.anchor.x();
    json["anchor y"] = m_state.anchor.y();
    json["center_x"] = m_state.center.x();
    json["center_y"] = m_state.center.y();
    json["rotation"] = m_state.rotation;
    json["brightness"] = m_state.brightness;
    json["contrast"] = m_state.contrast;
    json["gamma"] = m_state.gamma;

    m_unsaved_changes = false;
}

bool Bookmark::is_saved() const{
    return !m_unsaved_changes;
}

/**
 * @brief Bookmark::remove_exported_image
 * Removes the exported image, if there is one.
 */
// TODO update, currently empty
void Bookmark::remove_exported_image() {

}
