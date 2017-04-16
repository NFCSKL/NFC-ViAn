#include "bookmark.h"

/**
 * @brief Bookmark::Bookmark
 * @param frame_nbr Frame number associated with the bookmark.
 * @param file_pth Path to the stored image.
 * @param string Text description of the bookmark.
 */
Bookmark::Bookmark(int frame_nbr, QString file_pth, QString string) {
    this->frame_number = frame_nbr;
    this->file_path = file_pth;
    this->description = string;
}

Bookmark::Bookmark()
{
    frame_number = 0;
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
 * @brief Bookmark::get_file_path
 * @return Returns the file path to the stored image associated with the bookmark.
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
 * @brief Bookmark::read
 * @param json
 */
void Bookmark::read(const QJsonObject& json){
    this->frame_number = json["frame"].toInt();
    this->file_path = json["path"].toString();
    this->description = json["note"].toString();


}
/**
 * @brief Bookmark::write
 * @param json
 */
void Bookmark::write(QJsonObject& json){
    json["frame"] = this->frame_number;
    json["path"] = this->file_path;
    json["note"] = this->description;
}
