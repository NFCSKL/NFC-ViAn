#include "video.h"

Video::Video()
{
    this->filepath = "";
    this->id = -1;
}
Video::Video(ID id, std::string filepath){
    this->id = id;
    this->filepath = filepath;
}

std::stringstream& operator>>(std::stringstream& is, Video& vid){
    std::cout << "*********" << std::endl;
    std::cout << "IN VIDEO >>" << std::endl;
    std::cout << "*********" << std::endl;
    is >> vid.filepath;
    is >> vid.id;
    return is;
}

std::stringstream& operator<<(std::stringstream& os, const Video& vid){
    os << vid.id << " " << vid.filepath << " ";
    return os;
}
bool operator==(Video v1, Video v2){
    return v1.id == v2.id &&
           v1.filepath == v2.filepath;
}
