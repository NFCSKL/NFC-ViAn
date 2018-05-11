#include "imagesequence.h"

/**
 * @brief ImageSequence::ImageSequence
 * @param name: name of the sequence
 */
ImageSequence::ImageSequence(const std::string& path) : Video(true){
    seq_path = path;
    int index = path.find_last_of('/') + 1;
    m_name = file_path.substr(index);
}

/**
 * @brief ImageSequence::ImageSequence
 * @param name: name of the sequence
 * @param images: vector of paths to images
 */
ImageSequence::ImageSequence(const std::string& path, const std::vector<std::string> &images)
    : Video(true) {
    seq_path = path;
    int index = path.find_last_of('/') + 1;
    m_name = path.substr(index);
    m_images = images;
    file_path = seq_path + "/" + get_pattern_name();
    std::cout << file_path << std::endl;
}

/**
 * @brief ImageSequence::get_images
 * @return vector of image paths
 */
std::vector<std::string> ImageSequence::get_images(){
    return m_images;
}

/**
 * @brief ImageSequence::get_image_names
 * @return returns the file names of each item in the sequence (including suffix)
 */
std::vector<std::string> ImageSequence::get_image_names() {
    std::vector<std::string> names;
    for (auto image_path : m_images){
        int index = image_path.find_last_of('/') + 1;
        names.push_back(image_path.substr(index));
    }
    return names;
}

/**
 * @brief ImageSequence::get_pattern_name
 * Returns the string neccessary for opencv's VideoCapture class to open and play the sequence.
 * The string is a path relative the project path and need as such be appended to that before playback.
 * @return sequence pattern
 */
std::string ImageSequence::get_pattern_name() {
    int digits = Utility::number_of_digits(length());
    return "%0" + std::to_string(digits) + "d.jpg";
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
    seq_path = json["seq_path"].toString().toStdString();
    QJsonArray images = json["images"].toArray();
    for(auto item : images){
        m_images.push_back(item.toString().toStdString());
        std::cout << item.toString().toStdString() << std::endl;
    }
}

void ImageSequence::write(QJsonObject &json) {
    Video::write(json);
    json["seq_path"] = QString::fromStdString(seq_path);
    std::cout << "NAME" << m_name << std::endl;
    QJsonArray images;
    for (auto image_path : m_images){
        images.append(QString::fromStdString(image_path));
    }
    json["images"] = images;
}

void ImageSequence::add_image(const std::string &image_path, const int& index) {
    if (index == -1) {m_images.push_back(image_path);}
}
