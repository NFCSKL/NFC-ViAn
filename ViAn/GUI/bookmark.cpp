#include "bookmark.h"

/**
 * @brief Bookmark::Bookmark
 * @param frame_nbr Frame number associated with the bookmark.
 * @param file_pth Path to the stored image.
 * @param string Text description of the bookmark.
 */
Bookmark::Bookmark(int frame_nbr, std::string file_pth, QString string) {
    frame_number = frame_nbr;
    file_path = file_pth;
    description = string;
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
std::string Bookmark::get_file_path() {
    return file_path;
}

/**
 * @brief Bookmark::get_description
 * @return Returns the description associated with the bookmark.
 */
QString Bookmark::get_description() {
    return description;
}
