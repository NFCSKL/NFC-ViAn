#include "imagesequence.h"

/**
 * @brief ImageSequence::on_save
 * Should be called when the sequence is saved.
 * Updates the order tracking variables
 */
void ImageSequence::on_save() {
    m_saved_order = m_unsaved_order;
    //TODO remove items that no longer are in the sequence
    // update original images
}

/**
 * @brief ImageSequence::ImageSequence
 * @param name: name of the sequence
 */
ImageSequence::ImageSequence(const std::string& path) : Video(true){
    m_seq_path = path;
    int index = path.find_last_of('/') + 1;
    m_name = file_path.substr(index);
}

/**
 * @brief ImageSequence::ImageSequence
 * @param name: name of the sequence
 * @param images: vector of paths to images
 */
ImageSequence::ImageSequence(const std::string& path, const std::vector<std::string>& images, const std::vector<std::string>& checksums)
    : Video(path, true) {

    // Store image order and original file paths
    for (size_t i = 0; i < checksums.size(); ++i) {
        m_unsaved_order[checksums[i]] = i;
        m_original_images[checksums[i]] = images[i];
    }

    m_seq_path = path;
    file_path = path + "/" + get_pattern_name();
}

/**
 * @brief ImageSequence::get_search_path
 * @return std::string with the actual disc path to the folder containing the images
 */
std::string ImageSequence::get_search_path() const {
    return m_seq_path;
}

/**
 * @brief ImageSequence::get_pattern_name
 * Returns the string neccessary for opencv's VideoCapture class to open and play the sequence.
 * The string is a path relative the project path.
 * @return sequence pattern
 */
std::string ImageSequence::get_pattern_name() {
    int digits = Utility::number_of_digits(length());
    return "%0" + std::to_string(digits) + "d";
}

/**
 * @brief ImageSequence::get_saved_order
 * Returns the saved order of the sequence
 * @return std::map key: checksum of file, value: index in sequence
 */
std::map<std::string, int> ImageSequence::get_saved_order() const {
    return m_saved_order;
}

/**
 * @brief ImageSequence::get_unsaved_order
 * Returns the unsaved order of the sequence
 * @return std::map key: checksum of file, value: index in sequence
 */
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
            hashes.push_back((*it).first);
        }
    }
    return hashes;
}

/**
 * @brief ImageSequence::get_index_of_hash
 * @param hash: file hash/checksum
 * @return int the index in of the hash in the unsaved order
 */
int ImageSequence::get_index_of_hash(const std::string &hash) const {
    return m_unsaved_order.at(hash);
}

/**
 * @brief ImageSequence::get_hash_origin
 * Returns the path to the original file mapped to the hash/checksum
 * @param hash
 * @return std::string containing original image path
 */
std::string ImageSequence::get_original_name_from_hash(const std::string &hash) const {
    return Utility::name_from_path(m_original_images.at(hash));
}

/**
 * @brief ImageSequence::remove_image_with_hash
 * Removes the image with hash from the unsaved order
 * Also renames the actual file on disc (not removed so it can be "restored" and added to the sequence again)
 * @param hash
 * @return bool success
 */
bool ImageSequence::remove_image_with_hash(const std::string &hash) {
    auto it = m_unsaved_order.find(hash);
    if (it == m_unsaved_order.end()) return false;
    m_unsaved_order.erase(it);
    // TODO rename file to some temp name
    return true;
}

/**
 * @brief ImageSequence::length
 * @return int length of the sequence
 */
int ImageSequence::length(){
    return m_unsaved_order.size();
}

/**
 * @brief ImageSequence::read
 * Reads members from json object
 * @param json
 */
void ImageSequence::read(const QJsonObject &json) {
    Video::read(json);
    m_seq_path = json["seq_path"].toString().toStdString();

    // read sequence order
    QJsonObject order = json["order"].toObject();
    for (auto key : order.keys()) {
        m_saved_order[key.toStdString()] = order.value(key).toInt();
    }
    m_unsaved_order = m_saved_order;

    // read sequence order
    QJsonObject original_images = json["original_images"].toObject();
    for (auto key : original_images.keys()) {
        m_original_images[key.toStdString()] = original_images.value(key).toString().toStdString();
    }
}

/**
 * @brief ImageSequence::write
 * Writes members to json object
 * @param json
 */
void ImageSequence::write(QJsonObject &json) {
    on_save();
    Video::write(json);
    json["seq_path"] = QString::fromStdString(m_seq_path);

    QJsonObject order, original_images;
    for (auto it = m_saved_order.begin(); it != m_saved_order.end(); ++it) {
        order[QString::fromStdString((*it).first)] = (*it).second;
        original_images[QString::fromStdString((*it).first)] = QString::fromStdString(m_original_images[(*it).first]);
    }
    json["order"] = order;
    json["original_images"] = original_images;
}

void ImageSequence::add_image(const std::string &image_path, const int& index) {
    Q_UNUSED(image_path);
    Q_UNUSED(index);
// SAVE CODE INCASE OF REIMPLEMENTATION
//    if (index == -1) {
//        m_images.push_back(image_path);
//    } else {
//        m_images.insert(m_images.begin() + index, image_path);
//    }
}

void ImageSequence::reorder_elem(const int &from, const int &to) {
    Q_UNUSED(from);
    Q_UNUSED(to);
// SAVE CODE INCASE OF REIMPLEMENTATION
//    if (!(from >= 0 && to >= 0 && from < m_images.size() && to <= m_images.size())) return;
//    auto tmp = m_images.at(from);
//    m_images.insert(m_images.begin() + to, tmp);
//    if (to < from) {
//        m_images.erase(m_images.begin() + from + 1);
//    } else {
//        m_images.erase(m_images.begin() + from);
//    }
}
