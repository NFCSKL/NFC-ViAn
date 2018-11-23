#include "imagesequence.h"

#include "project.h"
#include "utility.h"

#include <QJsonArray>

/**
 * @brief ImageSequence::ImageSequence
 * @param name: name of the sequence
 */
ImageSequence::ImageSequence(const QString& path) : Video(true){
    seq_path = path;
    m_name = Utility::name_from_path(path);
}

/**
 * @brief ImageSequence::ImageSequence
 * @param name: name of the sequence
 * @param images: vector of paths to images
 */
ImageSequence::ImageSequence(const QString& path, const std::vector<QString> &images)
    : Video(true) {
    seq_path = path;
    m_name = Utility::name_from_path(path);
    m_images = images;
    file_path = seq_path + "/" + get_pattern_name();
}

/**
 * @brief ImageSequence::get_images
 * @return vector of image paths
 */
std::vector<QString> ImageSequence::get_images(){
    return m_images;
}

/**
 * @brief ImageSequence::get_image_names
 * @return returns the file names of each item in the sequence (including suffix)
 */
std::vector<QString> ImageSequence::get_image_names() {
    std::vector<QString> names;
    for (auto image_path : m_images){
        names.push_back(Utility::name_from_path(image_path));
    }
    return names;
}

/**
 * @brief ImageSequence::get_pattern_name
 * Returns the string neccessary for opencv's VideoCapture class to open and play the sequence.
 * The string is a path relative the project path and need as such be appended to that before playback.
 * @return sequence pattern
 */
QString ImageSequence::get_pattern_name() {
    int digits = Utility::number_of_digits(length());
    return "%0" + QString::number(digits) + "d";
}

/**
 * @brief ImageSequence::length
 * @return amount of images
 */
int ImageSequence::length(){
    return m_images.size();
}

void ImageSequence::read(const QJsonObject &json) {
    Video::read(json);
    seq_path = json["seq_path"].toString();
    QJsonArray images = json["images"].toArray();
    for(auto item : images){
        m_images.push_back(item.toString());
    }
}

void ImageSequence::write(QJsonObject &json) {
    Video::write(json);
    json["seq_path"] = seq_path;
    QJsonArray images;
    for (auto image_path : m_images){
        images.append(image_path);
    }
    json["images"] = images;
}

void ImageSequence::add_image(const QString &image_path, const int& index) {
    if (index == -1) {
        m_images.push_back(image_path);
    } else {
        m_images.insert(m_images.begin() + index, image_path);
    }
}

void ImageSequence::reorder_elem(const int &from, const int &to) {
    if (!(from >= 0 && to >= 0 && from < m_images.size() && to <= m_images.size())) return;
    auto tmp = m_images.at(from);
    m_images.insert(m_images.begin() + to, tmp);
    if (to < from) {
        m_images.erase(m_images.begin() + from + 1);
    } else {
        m_images.erase(m_images.begin() + from);
    }
}

void ImageSequence::reset_root_dir(const QString &dir) {
    seq_path = dir + Constants::SEQUENCE_FOLDER +  Utility::name_from_path(seq_path);
    file_path = seq_path + "/" + get_pattern_name();
}

