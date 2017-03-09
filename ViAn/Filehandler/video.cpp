#include "video.h"
/**
 * @brief Video::Video
 */
Video::Video()
{
    this->filepath = "";
    this->id = -1;
}
/**
 * @brief Video::Video
 * @param id
 * @param filepath
 */
Video::Video(ID id, std::string filepath){
    this->id = id;
    this->filepath = filepath;
}
/** TODO:unfinished, read operator
 * @brief operator >>
 * @param is
 * @param vid
 * @return stringstream with read project information
 */
std::stringstream& operator>>(std::stringstream& is, Video& vid){
    is >> vid.filepath;
    is >> vid.id;
    return is;
}
/**
 * @brief operator <<
 * @param os
 * @param vid
 * @return stream
 * Writes project information to stringstream
 */
std::stringstream& operator<<(std::stringstream& os, const Video& vid){
    os << vid.id << " " << vid.filepath << " ";
    return os;
}
/**
 * @brief operator ==
 * @param v1
 * @param v2
 * @return bool, videos same information
 */
bool operator==(Video v1, Video v2){
    return v1.id == v2.id &&
           v1.filepath == v2.filepath;
}
