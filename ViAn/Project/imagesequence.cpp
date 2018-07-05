#include "imagesequence.h"

/**
 * @brief ImageSequence::on_save
 * Should be called when the sequence is saved.
 * Updates the order tracking variables
 */
void ImageSequence::on_save() {
    m_saved_order = m_unsaved_order;
    //TODO remove items that no longer are in the sequence
}

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

    // Go through each file in the sequence folder and hash it
    for (unsigned int i = 0; i < m_images.size(); ++i) {
        QString file_path = QString::fromStdString(seq_path + "/" + Utility::zfill(std::to_string(i), Utility::number_of_digits(m_images.size())));
        std::string hash = Utility::checksum(file_path).toStdString();
        m_unsaved_order[hash] = i;
        m_original_images[hash] = images[i];
        qDebug() << "Checksum created: " << QString::fromStdString(hash);
    }
}

std::string ImageSequence::get_container_path() const {
    return seq_path;
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
    return "%0" + std::to_string(digits) + "d";
}

std::map<std::string, int> ImageSequence::get_saved_order() const {
    return m_saved_order;
}

std::map<std::string, int> ImageSequence::get_unsaved_order() const {
    return m_unsaved_order;
}

/**
 * @brief ImageSequence::get_unsaved_hashes
 * Returns a vector of all checksums for the files that has been added but are unsaved.
 * @return
 */
std::vector<std::string> ImageSequence::get_unsaved_hashes() const {
    std::vector<std::string> hashes;
    for (auto it = m_unsaved_order.begin(); it != m_unsaved_order.end(); ++it) {
        if (m_saved_order.find((*it).first) == m_saved_order.end()) {
            qDebug() << QString::fromStdString((*it).first) << ": Not saved";
            hashes.push_back((*it).first);
        }
    }
    return hashes;
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
    }
}

void ImageSequence::write(QJsonObject &json) {
    on_save();
    Video::write(json);
    json["seq_path"] = QString::fromStdString(seq_path);
    QJsonArray images;
    for (auto image_path : m_images){
        images.append(QString::fromStdString(image_path));
    }
    json["images"] = images;

    QJsonObject order, original_images;
    for (auto it = m_saved_order.begin(); it != m_saved_order.end(); ++it) {
        order[QString::fromStdString((*it).first)] = (*it).second;
        original_images[QString::fromStdString((*it).first)] = QString::fromStdString(m_original_images[(*it).first]);
    }
    json["order"] = order;
    json["original_images"] = original_images;
}

void ImageSequence::add_image(const std::string &image_path, const int& index) {
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
