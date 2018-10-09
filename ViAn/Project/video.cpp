#include "video.h"

/**
 * @brief Video::Video
 */
Video::Video(const int& sequence_type){
    m_sequence_type = sequence_type;
    m_file_path = "";
}

/**
 * @brief Video::Video
 * @param id
 * @param filepath
 */
Video::Video(std::string file_path, const int& sequence_type){
    m_sequence_type = sequence_type;
    m_file_path = file_path;
    int index = file_path.find_last_of('/') + 1;
    m_name = file_path.substr(index);
}

Video::~Video() {}

std::string Video::get_name() {
    return m_name;
}

void Video::set_name(const std::string &new_name) {
    m_name = new_name;
}

int Video::get_sequence_type() {
    return m_sequence_type;
}

/**
 * @brief operator ==
 * @param v1
 * @param v2
 * @return if videos the same TRUE
 */
bool operator==(Video v1, Video v2){
    return v1.m_file_path == v2.m_file_path;
}

/**
 * @brief Video::read
 * @param json
 * Read video parameters from json object.
 */
void Video::read(const QJsonObject& json){
    m_sequence_type = json["sequence"].toBool();
    m_file_path = json["file_path"].toString().toStdString();
    m_name = json["name"].toString().toStdString();
}

/**
 * @brief Video::write
 * @param json
 * Write video parameters to json object.
 */
void Video::write(QJsonObject& json){
    json["sequence"] = m_sequence_type;
    json["name"] = QString::fromStdString(m_name);
    json["file_path"] = QString::fromStdString(m_file_path);
}
