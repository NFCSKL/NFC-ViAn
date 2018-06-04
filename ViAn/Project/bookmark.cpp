#include "bookmark.h"
#include "utility.h"

/**
 * @brief Bookmark::Bookmark
 * @param vid_proj video project for the bookmark
 * @param file_name file name for bookmark
 * @param text Text description of the bookmark.
 * @param frame_nbr Frame number associated with the bookmark.
 * @param time The time in the video associated with the bookmark (format "mm:ss").
 */
Bookmark::Bookmark(VideoProject *vid_proj, const std::string file_name, const std::string &text, const int &frame_nbr, const QString time){
    m_vid_proj = vid_proj;
    m_file = file_name;
    m_description = text;
    m_frame_nbr = frame_nbr;
    m_time = time;
    std::pair<int, std::string> _tmp(UNSORTED, "");
    m_container = _tmp;
}

/**
 * @brief Bookmark::Bookmark
 * Copy constructor
 * @param bookmark
 */
Bookmark::Bookmark(const Bookmark &bookmark) {
    m_vid_proj = bookmark.m_vid_proj;
    m_file = bookmark.m_file;
    m_description = bookmark.m_description;
    m_frame_nbr = bookmark.m_frame_nbr;
    m_time = bookmark.m_time;
    m_container == bookmark.m_container;
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
Bookmark::~Bookmark(){}


void Bookmark::reset_root_dir(const std::string &dir){    
    m_file = dir + Utility::name_from_path(m_file);
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
    return m_frame_nbr;
}

std::pair<int, std::string> Bookmark::get_container(){
    return m_container;
}

VideoProject *Bookmark::get_video_project() {
    return m_vid_proj;
}

/**
 * @brief Bookmark::get_description
 * @return Returns the description associated with the bookmark.
 */
std::string Bookmark::get_description() {
    return m_description;
}

/**
 * @brief Bookmark::set_description
 * Sets the text description of the bookmark to the specified string.
 * @param text
 */
void Bookmark::set_description(const std::string& text) {
    m_description = text;
    m_unsaved_changes = true;
}

void Bookmark::set_container(std::string name, int type) {
    std::pair<int, std::string> container(type, name);
    m_container = container;
    m_unsaved_changes = true;
}

/**
 * @brief Bookmark::rename_container
 * Changes the name of all the containers with old_name to new_name
 * @param old_name
 * @param new_name
 */
void Bookmark::rename_container(std::string old_name, std::string new_name) {
    if (m_container.second == old_name) {
        m_container.second = new_name;
    }
    m_unsaved_changes = true;
}

/**
 * @brief Bookmark::set_video_project
 * Sets the video project that the bookmark belogs to
 * @param vid_proj
 */
void Bookmark::set_video_project(VideoProject *vid_proj){
    m_vid_proj = vid_proj;
}

void Bookmark::add_to_video_project() {
    m_vid_proj->add_bookmark(this);
}

/**
 * @brief Bookmark::remove
 * Returns a bool specifying if the bookmark should be deleted or not
 * @return
 */
bool Bookmark::remove() {
    return false;
    //return (m_containers.empty());
}

/**
 * @brief Bookmark::read
 * @param json
 * Reads a bookmark from a Json object.
 */
void Bookmark::read(const QJsonObject& json){
    m_time = json["time"].toString();
    m_frame_nbr = json["frame"].toInt();
    m_file = json["path"].toString().toStdString();
    m_description = json["description"].toString().toStdString();
    std::pair<int, std::string> pair(json["type"].toInt(), json["container"].toString().toStdString());
    m_container = pair;
    
    m_unsaved_changes = false;
}

/**
 * @brief Bookmark::write
 * @param json
 * Writes a bookmark to a Json object.
 */
void Bookmark::write(QJsonObject& json){
    json["time"] = this->m_time;
    json["frame"] = this->m_frame_nbr;
    json["path"] = QString::fromStdString(m_file);
    json["description"] = QString::fromStdString(m_description);
    json["container"] =QString::fromStdString( m_container.second);
    json["type"] = m_container.first;

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
