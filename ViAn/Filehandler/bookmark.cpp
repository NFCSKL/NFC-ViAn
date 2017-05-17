#include "bookmark.h"

/**
 * @brief Bookmark::Bookmark
 * @param frame_nbr Frame number associated with the bookmark.
 * @param frame Frame associated with the bookmark.
 * @param video_file_name Name of the video associated with the bookmark.
 * @param dir_path Path to the directory to store image in.
 * @param text Text description of the bookmark.
 */
Bookmark::Bookmark(int frame_nbr, QImage frame, QString video_file_name, QString dir_path, QString text) {
    this->frame_number = frame_nbr;
    this->frame = frame;
    this->video_file_name = video_file_name;
    this->dir_path = dir_path;
    this->description = text;
    // There's no file path yet, since the frame has not been exported
    this->file_path = QString();
}

/**
 * @brief Bookmark::Bookmark
 * Null initializing constructor.
 */
Bookmark::Bookmark() {
    frame_number = 0;
    frame = QImage();
    video_file_name = QString();
    dir_path = QString();
    file_path = QString();
    description = QString();
}

/**
 * @brief Bookmark::get_frame_number
 * @return Returns the frame number that the bookmark points to.
 */
int Bookmark::get_frame_number() {
    return frame_number;
}

/**
 * @brief Bookmark::get_frame
 * @return Returns the frame of the bookmark.
 */
QImage Bookmark::get_frame() {
    return frame;
}

/**
 * @brief Bookmark::get_file_path
 * @return Returns the file_path of the exported bookmark.
 *         If bookmark is not exported yet, an empty string is returned
 */
QString Bookmark::get_file_path() {
    return file_path;
}

/**
 * @brief Bookmark::get_description
 * @return Returns the description associated with the bookmark.
 */
QString Bookmark::get_description() {
    return description;
}

/**
 * @brief Bookmark::set_description
 * Sets the text description of the bookmark to the specified string.
 * @param text
 */
void Bookmark::set_description(QString text) {
    this->description = text;
}

/**
 * @brief Bookmark::read
 * @param json
 * Reads a bookmark from a Json object.
 */
void Bookmark::read(const QJsonObject& json){
    this->frame_number = json["frame"].toInt();
    this->video_file_name = json["video_name"].toString();
    this->dir_path = json["dir"].toString();
    this->file_path = json["path"].toString();
    this->description = json["note"].toString();
    frame.load(file_path);
}

/**
 * @brief Bookmark::write
 * @param json
 * Writes a bookmark to a Json object, and exports the frame.
 */
void Bookmark::write(QJsonObject& json){
    // Exports the frame and updates file_path.
    export_frame();

    json["frame"] = this->frame_number;
    json["video_name"] = this->video_file_name;
    json["dir"] = this->dir_path;
    json["path"] = this->file_path;
    json["note"] = this->description;
}

/**
 * @brief Bookmark::export_frame
 * Export the frame of the bookmark to a tiff-file in the project folder.
 */
void Bookmark::export_frame() {
    // Update file path in case there's already a file with this file name
    create_file_path();
    QImageWriter writer(file_path, "tiff");
    writer.write(frame);
}

/**
 * @brief Bookmark::create_file_path
 * Creates and updates the file path to export the bookmark frame to.
 */
void Bookmark::create_file_path() {

    // Append FRAMENR.tiff to the directory path
    QString path = QString(dir_path);
    path.append("/");
    path.append(video_file_name);
    path.append("_");
    path.append(QString::number(frame_number));
    path.append(".tiff");
         
    int counter = 1;
    while (QFile::exists(path)) {
        // If file exists, try FRAMENR(X).tiff
        path = QString(dir_path);
        path.append("/");
        path.append(video_file_name);
        path.append("_");
        path.append(QString::number(frame_number));
        path.append("(");
        path.append(QString::number(counter));
        path.append(").tiff");
        counter++;
    }
    // Update file path variable
    file_path = path;
}

/**
 * @brief Bookmark::remove_exported_image
 * Removes the exported image, if there is one.
 */
void Bookmark::remove_exported_image() {
    // If the file path is empty, then the frame has not been exported so there's nothing to remove.
    if (!file_path.isEmpty()) {
        QFile file(file_path);
        file.remove();
    }
}
