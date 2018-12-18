#include "video.h"
#include "utility.h"

/**
 * @brief Video::Video
 */
Video::Video(const VIDEO_TYPE& sequence_type){
    m_sequence_type = sequence_type;
    this->file_path = "";
}

/**
 * @brief Video::Video
 * @param id
 * @param filepath
 */
Video::Video(QString file_path, const VIDEO_TYPE& sequence_type){
    m_sequence_type = sequence_type;
    this->file_path = file_path;
    m_name = Utility::name_from_path(file_path);
    m_is_saved = (m_sequence_type == VIDEO); // Ordinary videos can't be changed thus are allways "saved"
}

Video::~Video() {}

QString Video::get_name() {
    return m_name;
}

void Video::set_name(const QString &new_name) {
    m_name = new_name;
}

bool Video::is_sequence() {
    return m_sequence_type != VIDEO;
}

VIDEO_TYPE Video::get_sequence_type() {
    return m_sequence_type;
}

bool Video::is_saved() {
    return m_is_saved;
}

int Video::get_width() {
    return m_width;
}

int Video::get_height() {
    return m_height;
}

void Video::set_size(int width, int height) {
    m_width = width;
    m_height = height;
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
    m_sequence_type = static_cast<VIDEO_TYPE>(json["sequence"].toInt());
    this->file_path = json["file_path"].toString();
    m_name = json["name"].toString();
    m_is_saved = true;
}

/**
 * @brief Video::write
 * @param json
 * Write video parameters to json object.
 */
void Video::write(QJsonObject& json){
    json["sequence"] = m_sequence_type;
    json["name"] = m_name;
    json["file_path"] = this->file_path;
    m_is_saved = true;
}

void VideoState::read(const QJsonObject& json) {
    frame = json["frame"].toInt();
    scale_factor = json["scale_factor"].toDouble();
    int x = json["anchor x"].toInt();
    int y = json["anchor y"].toInt();
    anchor = QPoint(x, y);
    x = json["center_x"].toInt();
    y = json["center_y"].toInt();
    center = QPoint(x, y);
    rotation = json["rotation"].toInt();
    brightness = json["brightness"].toInt();
    contrast = json["contrast"].toDouble();
    gamma = json["gamma"].toDouble();
}

void VideoState::write(QJsonObject& json) {
    json["frame"] = frame;
    json["scale_factor"] = scale_factor;
    json["anchor x"] = anchor.x();
    json["anchor y"] = anchor.y();
    json["center_x"] = center.x();
    json["center_y"] = center.y();
    json["rotation"] = rotation;
    json["brightness"] = brightness;
    json["contrast"] = contrast;
    json["gamma"] = gamma;
}
