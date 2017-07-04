#include "bookmark.h"

/**
 * @brief Bookmark::Bookmark
 * @param time The time in the video associated with the bookmark (in millisecs).
 * @param frame_nbr Frame number associated with the bookmark.
 * @param frame Frame associated with the bookmark.
 * @param video_file_name Name of the video associated with the bookmark.
 * @param dir_path Path to the directory to store image in.
 * @param text Text description of the bookmark.
 */
Bookmark::Bookmark(VideoProject *vid_proj, const std::string &text, const int &frame_nbr){
    this->m_vid_proj = vid_proj;
    this->frame_number = frame_nbr;
    this->description = text;
}

/**
 * @brief Bookmark::Bookmark
 * Null initializing constructor.
 */
Bookmark::Bookmark() {
}

/**
 * @brief Bookmark::get_time
 * @return Returns the time in the video where the bookmark points to (in millisecs).
 */
int Bookmark::get_time() {
    return time;
}

/**
 * @brief Bookmark::get_frame_number
 * @return Returns the frame number that the bookmark points to.
 */
int Bookmark::get_frame_number() {
    return frame_number;
}


int Bookmark::get_type() {
    return m_type;
}

/**
 * @brief Bookmark::get_container_name
 * @return
 */
std::string Bookmark::get_container_name() {
    return m_container_name;
}


/**
 * @brief Bookmark::get_description
 * @return Returns the description associated with the bookmark.
 */
std::string Bookmark::get_description() {
    return description;
}

/**
 * @brief Bookmark::set_description
 * Sets the text description of the bookmark to the specified string.
 * @param text
 */
void Bookmark::set_description(const std::string& text) {
    description = text;
}

/**
 * @brief Bookmark::set_type
 * Sets the bookmark type
 * @param type
 */
void Bookmark::set_type(const int type) {
    m_type = type;
}

/**
 * @brief Bookmark::set_container_name
 * Sets the name of the GUI container that the bookmark is stored in
 * @param name
 */
void Bookmark::set_container_name(const string name) {
    m_container_name = name;
}

/**
 * @brief Bookmark::read
 * @param json
 * Reads a bookmark from a Json object.
 */
void Bookmark::read(const QJsonObject& json){
    this->time = json["time"].toInt();
    this->frame_number = json["frame"].toInt();
    m_file = json["path"].toString().toStdString();
    this->description = json["description"].toString().toStdString();
    m_type = json["type"].toInt();
    m_container_name = json["container"].toString().toStdString();

}

/**
 * @brief Bookmark::write
 * @param json
 * Writes a bookmark to a Json object, and exports the frame.
 */
void Bookmark::write(QJsonObject& json){
    // Exports the frame and updates file_path.
    json["time"] = this->time;
    json["frame"] = this->frame_number;
    json["path"] = QString::fromStdString(m_file);
    json["description"] = QString::fromStdString(description);
    json["type"] = m_type;
    json["container"] = QString::fromStdString(m_container_name);
}

/**
 * @brief Bookmark::export_frame
 * Export the frame of the bookmark to a tiff-file in the project folder.
 */
void Bookmark::export_frame() {
// Update file path in case there's already a file with this file name
//    create_file_path();
//    QImageWriter writer(file_path, "tiff");
//    writer.write(frame);
}

/**
 * @brief Bookmark::create_file_path
 * Creates and updates the file path to export the bookmark frame to.
 */
void Bookmark::create_file_path() {
//    // Append FRAMENR.tiff to the directory path
//    QString path = QString(dir_path);
//    path.append("/");
//    path.append(video_file_name);
//    path.append("_");
//    path.append(QString::number(frame_number));
//    path.append(".tiff");
         
//    int counter = 1;
//    while (QFile::exists(path)) {
//        // If file exists, try FRAMENR(X).tiff
//        path = QString(dir_path);
//        path.append("/");
//        path.append(video_file_name);
//        path.append("_");
//        path.append(QString::number(frame_number));
//        path.append("(");
//        path.append(QString::number(counter));
//        path.append(").tiff");
//        counter++;
//    }
//    // Update file path variable
//    m_full_path = path;
}

/**
 * @brief Bookmark::remove_exported_image
 * Removes the exported image, if there is one.
 */
void Bookmark::remove_exported_image() {
    delete_saveable();
}
