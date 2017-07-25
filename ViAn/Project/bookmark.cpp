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

    std::pair<int, string> _tmp(UNSORTED, "");
    m_containers.push_back(_tmp);
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

std::vector<std::pair<int, string> > Bookmark::get_containers(){
    return m_containers;
}

VideoProject *Bookmark::get_video_project() {
    return m_vid_proj;
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

void Bookmark::add_container(string name, int type) {
    std::pair<int, std::string> container(type, name);
    m_containers.push_back(container);
}

void Bookmark::remove_container(string name, int type) {
    std::pair<int, std::string> container(type, name);
    auto cont = std::find(m_containers.begin(), m_containers.end(), container);
    if (cont != m_containers.end()) m_containers.erase(cont);
}

/**
 * @brief Bookmark::rename_container
 * Changes the name of all the containers with old_name to new_name
 * @param old_name
 * @param new_name
 */
void Bookmark::rename_container(std::string old_name, std::string new_name) {
    std::for_each(m_containers.begin(), m_containers.end(),
                  [&old_name, &new_name](std::pair<int, std::string> &c){if (c.second == old_name) c.second = new_name;});
}

/**
 * @brief Bookmark::set_video_project
 * Sets the video project that the bookmark belogs to
 * @param vid_proj
 */
void Bookmark::set_video_project(VideoProject *vid_proj){
    m_vid_proj = vid_proj;
}

/**
 * @brief Bookmark::remove
 * Returns a bool specifying if the bookmark should be deleted or not
 * @return
 */
bool Bookmark::remove() {
    return (m_containers.empty());
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

    QJsonArray containers = json["containers"].toArray();
    for (int i = 0; i < containers.size(); ++i) {
        QJsonObject container = containers[i].toObject();
        std::pair<int,std::string> _tmp(container["type"].toInt(), container["container"].toString().toStdString());
        m_containers.push_back(_tmp);
    }

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

    QJsonArray containers;
    for (auto it = m_containers.begin(); it != m_containers.end(); ++it) {
        QJsonObject container;
        std::pair<int,std::string> pair = *it;
        container["container"] = QString::fromStdString(pair.second);
        container["type"] = pair.first;
        containers.push_back(container);
    }
    json["containers"] = containers;
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
}
