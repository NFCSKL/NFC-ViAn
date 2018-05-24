#include "video.h"

/**
 * @brief Video::Video
 */
Video::Video(){
    this->file_path = "";
}

/**
 * @brief Video::Video
 * @param id
 * @param filepath
 */
Video::Video(std::string file_path){
    this->file_path = file_path;
    int index = file_path.find_last_of('/') + 1;
    m_name = file_path.substr(index);
}

Video::~Video() {}

std::string Video::get_name() {
    return m_name;
}

void Video::set_zoom_rect(QPoint start, QPoint end) {
    state.zoom_start = start;
    state.zoom_end = end;
}

/**
 * @brief operator ==
 * @param v1
 * @param v2
 * @return if videos the same TRUE
 */
bool operator==(Video v1, Video v2){
    return v1.file_path == v2.file_path;
}

/**
 * @brief Video::read
 * @param json
 * Read video parameters from json object.
 */
void Video::read(const QJsonObject& json){
    this->file_path = json["file_path"].toString().toStdString();
    m_name = json["name"].toString().toStdString();
}

/**
 * @brief Video::write
 * @param json
 * Write video parameters to json object.
 */
void Video::write(QJsonObject& json){
    json["file_path"] = QString::fromStdString(this->file_path);
    json["name"] = QString::fromStdString(m_name);

}
