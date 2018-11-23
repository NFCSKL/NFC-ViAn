#include "imagesequence.h"
#include "project.h"
#include <algorithm>
#include <QDebug>

/**
 * @brief ImageSequence::update
 * Updates the variables that keeps track of sequence changes.
 * If a file hash is stored in the saved order but not in the unsaved order
 * that file will be removed.
 *
 * This function is meant to be called before saving to file
 */
void ImageSequence::update() {
    // If a hash from the saved order no longer exists in the unsaved order the file should be removed.
    // If this happens the file should be saved as its hash instead of index
    for (auto it = m_saved_order.begin(); it != m_saved_order.end(); ++it) {
        if (m_unsaved_order.find((*it).first) == m_unsaved_order.end()){
            if (!remove_image_from_disc((*it).first))
                qWarning() << "failed to remove";
        }
    }
    m_saved_order = m_unsaved_order;
    is_new = false;
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
    is_new = true;
}

/**
 * @brief ImageSequence::get_search_path
 * Returns the absolute path to the parent folder of the images.
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
//    int digits = INDEX_LENGTH;//Utility::number_of_digits(length());
    return "%0" + std::to_string(INDEX_LENGTH) + "d";
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
 * CURRENTLY NOT USED
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
 * Returns the image index for the given file hash
 * @param hash: file hash/checksum
 * @return int the index in of the hash in the unsaved order
 */
int ImageSequence::get_index_of_hash(const std::string &hash) const {
    return m_unsaved_order.at(hash);
}

/**
 * @brief ImageSequence::get_original_name_from_hash
 * Returns the path to the original file mapped to the hash/checksum
 * @param hash
 * @return std::string containing original image path
 */
std::string ImageSequence::get_original_name_from_hash(const std::string &hash) const {
    return Utility::name_from_path(m_original_images.at(hash));
}

std::string ImageSequence::get_original_name_from_index(const int& index) const {
    std::string hash = "Invalid path";
    bool found_hash = false;
    for (auto elem : m_unsaved_order) {
        if (elem.second == index) {
            hash = elem.first;
            found_hash = true;
            break;
        }
    }
    if (found_hash) {
        return m_original_images.at(hash);
    } else {
        return hash;
    }
}

bool ImageSequence::never_saved() const {
    return is_new;
}

/**
 * @brief ImageSequence::remove_image_with_hash
 * Removes the image with the corresponding hash from the unsaved order
 * Also renames the image file to the hash string and shifts the indices of all images coming after it.
 * @param hash
 * @return bool whether the given hash exists or not
 */
bool ImageSequence::remove_image_with_hash(const std::string &hash) {
    if (m_saved_order.empty()) return true;
    auto it = m_unsaved_order.find(hash);
    if (it == m_unsaved_order.end()) return false;
    auto hash_idx = (*it);
    m_unsaved_order.erase(it);

    int index = hash_idx.second;
    rename_image_on_disc(Utility::zfill(index, INDEX_LENGTH), hash_idx.first);
    std::vector<std::pair<std::string, int>> unsaved;

    // Sort the unsaved vector by the indices (the value)
    for (auto it = m_unsaved_order.begin(); it != m_unsaved_order.end(); ++it) {
        unsaved.push_back((*it));
    }
    auto cmp = [](std::pair<std::string,int> const & a, std::pair<std::string,int> const & b) {
        return a.second < b.second;
    };
    std::sort(unsaved.begin(), unsaved.end(), cmp);

    // Shift down the indices of all images coming after the image with the given hash
    for (auto key_val : unsaved) {
        int i = key_val.second;
        if (i > index) {
            rename_image_on_disc(Utility::zfill(i, INDEX_LENGTH), Utility::zfill(i - 1, INDEX_LENGTH));
            m_unsaved_order[key_val.first] = i - 1;
        }
    }
    m_is_saved = false;
    return true;
}

/**
 * @brief ImageSequence::remove_image_from_disc
 * Removes the given image from disc
 * @param image_name the name of the image file
 * @return bool whether the file was removed or not
 */
bool ImageSequence::remove_image_from_disc(const std::string &image_name) const {
    QString base_path = QString::fromStdString(m_seq_path + "/");
    return QFile().remove(base_path + QString::fromStdString(image_name));
}

/**
 * @brief ImageSequence::rename_image_on_disc
 * Renames the file with old name to new name
 * @param old_name  Current name of file
 * @param new_name  Name to change to
 * @return bool     whether the rename was successful or not
 */
bool ImageSequence::rename_image_on_disc(const std::string &old_name, const std::string &new_name) const {
    QString base_path = QString::fromStdString(m_seq_path + "/");
    return QFile().rename(base_path + QString::fromStdString(old_name),
                        base_path + QString::fromStdString(new_name));
}

/**
 * @brief ImageSequence::length
 * Return the current length of the sequence
 * @return int length of the sequence
 */
int ImageSequence::length(){
    return m_unsaved_order.size();
}

/**
 * @brief ImageSequence::restore
 * Restores the image sequnce to the last saved order by
 * renaming all files on disc to the indices stored in the
 * save order map.
 * If the sequence never has been saved all files will be removed.
 */
void ImageSequence::revert() {
    QString base_path = QString::fromStdString(m_seq_path + "/");
    // Rename all files currently in the sequence to their file hash
    for (auto it = m_unsaved_order.begin(); it != m_unsaved_order.end(); ++it) {
       QFile::rename(base_path + QString::fromStdString(Utility::zfill((*it).second, INDEX_LENGTH)),
                      base_path + QString::fromStdString((*it).first));

        // Remove file if it does not exist in the saved order
        // i.e remove unsaved files
        if (m_saved_order.find((*it).first) == m_saved_order.end())
            QFile::remove(base_path + QString::fromStdString((*it).first));
    }

    // Rename all file to the indices stored in the saved order
    for (auto it = m_saved_order.begin(); it != m_saved_order.end(); ++it) {
        QFile::rename(base_path + QString::fromStdString((*it).first),
                      base_path + QString::fromStdString(Utility::zfill((*it).second, INDEX_LENGTH)));
    }

    // TODO Remove any files not in the saved order
    // This will be needed when functionality exists for adding new images
    m_unsaved_order = m_saved_order;
}

/**
 * @brief ImageSequence::read
 * Loads a image sequence from json object (save file)
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
 * Writes the image sequence data to json
 * so it can be loaded later
 * @param json
 */
void ImageSequence::write(QJsonObject &json) {
    update();
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

/**
 * @brief ImageSequence::add_image
 * @param image_path
 * @param index
 */
void ImageSequence::add_image(const std::string &image_path, const int& index) {
    Q_UNUSED(image_path);
    Q_UNUSED(index);
    Q_UNIMPLEMENTED();
}

void ImageSequence::reorder_elem(const int &from, const int &to) {
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNIMPLEMENTED();
}

void ImageSequence::reset_root_dir(const std::string &dir) {
    m_seq_path = dir + Project::SEQUENCE_FOLDER +  Utility::name_from_path(m_seq_path);
    file_path = m_seq_path + "/" + get_pattern_name();
}
