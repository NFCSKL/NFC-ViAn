#include "bookmark.h"

/**
 * @brief Bookmark::Bookmark
 * @param frame_nbr Frame number associated with the bookmark.
 * @param frame Frame associated with the bookmark.
 * @param dir_path Path to the directory to store image in.
 * @param string Text description of the bookmark.
 */
Bookmark::Bookmark(int frame_nbr, QImage frame, QString dir_path, QString string) {
    this->frame_number = frame_nbr;
    this->frame = frame;
    this->dir_path = dir_path;
    this->description = string;

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
 * @brief Bookmark::get_description
 * @return Returns the description associated with the bookmark.
 */
QString Bookmark::get_description() {
    return description;
}

/**
 * @brief Bookmark::read
 * @param json
 * Reads a bookmark from a Json object.
 */
void Bookmark::read(const QJsonObject& json){
    this->frame_number = json["frame"].toInt();
    this->file_path = json["path"].toString();
    this->description = json["note"].toString();
    frame.load(file_path);
}

/**
 * @brief Bookmark::write
 * @param json
 * Writes a bookmark to a Json object.
 */
void Bookmark::write(QJsonObject& json){
    json["frame"] = this->frame_number;
    json["path"] = this->file_path;
    json["note"] = this->description;
    export_frame();
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
    path.append(QString::number(frame_number));
    path.append(".tiff");

    int counter = 1;
    while (QFile::exists(path)) {
        // If file exists, try FRAMENR(X).tiff
        path = QString(dir_path);
        path.append("/");
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
 * @brief Bookmark::delete_artifact
 * Deletes the exported image.
 */
void Bookmark::delete_exported_image() {
    QFile file(file_path);
    file.remove();
}
