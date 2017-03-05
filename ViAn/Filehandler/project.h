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
   std::string f_proj;
   std::string f_analysis;
   std::string f_drawings;
   std::string f_videos;
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
    void addVideo(Video *vid);

    friend std::ifstream& operator>>(std::stringstream& is, Project& proj);
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
    ID m_dir;
    ID m_file;
    std::vector<Video*> m_videos;
};


#endif // PROJECT_H
