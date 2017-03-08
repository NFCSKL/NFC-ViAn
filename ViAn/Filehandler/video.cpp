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
/**
 * @brief operator >>
 * @param is
 * @param vid
 * @return UNFINISHED
 */
std::stringstream& operator>>(std::stringstream& is, Video& vid){
    std::string hej;
    is >>hej;
    std::cout << "********"<< hej << "********";
    is >> vid.id;
    is >> vid.filepath;
    return is;
}
/**
 * @brief operator <<
 * @param os
 * @param vid
 * @return used for writing project to file
 */
std::stringstream& operator<<(std::stringstream& os, const Video& vid){
    os << vid.id << " " << vid.filepath << " ";
    return os;
}
/**
 * @brief operator ==
 * @param v1
 * @param v2
 * @return if videos the same TRUE
 *
 */
bool operator==(Video v1, Video v2){
    return v1.id == v2.id &&
           v1.filepath == v2.filepath;
}
