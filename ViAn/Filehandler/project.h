#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include "filehandler.h"
#include "video.h"
typedef int ID;

struct ProjFiles{
   ID dir;
   ID f_proj;
   ID f_analysis;
   ID f_drawings;
   ID f_videos;
   ProjFiles(){
       this->dir = -1;
       this->f_proj = -1;
       this->f_analysis = -1;
       this->f_drawings = -1;
       this->f_videos = -1;
   }
};

/**
 * @brief The Project struct
 * incomplete struct, will be added on
 * along with parser functionality
 */

struct Project{
public:
    Project();
    Project(ID id, std::string name);
    Project(std::string dirpath);
    void add_video(Video *vid);

    friend std::stringstream& operator>>(std::stringstream& is, Project& proj);
    friend std::stringstream& operator<<(std::stringstream& os, Project const& proj);
    friend bool operator==(Project proj, Project proj2);

// TODO
//    void addAnalysis();
//    void addDrawing();
private:
    ID m_vid;        
public:
    ID m_id;
    std::string m_name;
    ProjFiles* files;
    std::vector<Video*> m_videos;
};


#endif // PROJECT_H
