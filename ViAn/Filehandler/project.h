#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <iostream>
#include <fstream>
#include "filehandler.h"

typedef int ID;
struct Video{
    ID id;
    std::string filepath;

    Video(ID id, std::string filepath){
        this->id = id;
        this->filepath = filepath;
    }
};
/**
 * @brief The Project struct
 * incomplete struct, will be added on
 * along with parser functionality
 */
struct Project{
public:
    Project(ID id, std::string name);
    Project(std::string dirpath);
    void addVideo(Video vid);
// TODO
//    void addAnalysis();
//    void addDrawing();
private:
    ID m_vid;    
public:
    ID m_id;
    std::string m_name;
    ID m_dir;
    ID m_file;
    std::vector<Video> m_videos;

};
#endif // PROJECT_H
