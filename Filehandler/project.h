#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <windows.h>
#include <iostream>
#include <fstream>
#include "stringhelper.h"
#include "filehandler.h"
typedef int Analysis;
typedef int ID;
typedef size_t Drawing;

struct Video{
    ID id;
    std::vector<Analysis> analyses;
    std::vector<Drawing> drawings;   
    Video(ID id){
        this->id = id;
    }
};

class Project{
public:
    Project(ID id, std::string name);
    Project(std::string dirpath);
    ~Project();
    void addVideo(Video vid);
    void open();
    void close();
    void remove();
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
