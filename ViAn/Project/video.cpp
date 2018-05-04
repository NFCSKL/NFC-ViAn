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

Video::Video(std::string file_path, const int &total_frames, const int &total_time){
    std::cout << "File: " << file_path << std::endl;
    this->file_path = file_path;
    int index = file_path.find_last_of('/') + 1;
    m_name = file_path.substr(index);

    m_total_frames = total_frames;
    m_total_time = total_time;
}

std::string Video::get_name() {
    return m_name;
}

int Video::get_frames() {
    return m_total_frames;
}

int Video::get_time() {
    return m_total_time;
}

void Video::set_frames(const int& num_frames){
    m_total_frames = num_frames;
}

void Video::set_time(const int& time_ms) {
    m_total_time = time_ms;
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
    m_total_frames = json["frame_count"].toInt();
    m_total_time = json["duration"].toInt();
}

/**
 * @brief Video::write
 * @param json
 * Write video parameters to json object.
 */
void Video::write(QJsonObject& json){
    json["file_path"] = QString::fromStdString(this->file_path);
    json["name"] = QString::fromStdString(m_name);
    json["frame_count"] = m_total_frames;
    json["duration"] = m_total_time;
}
