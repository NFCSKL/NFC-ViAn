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
#include <QJsonObject>
#include <QJsonArray>
typedef int ID;

/**
 * @brief The ProjFiles struct
 * project file container, simplifies code for readability,
 * also easier to pass all files as opposed to every file
 * seperately
 */
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
    Project(std::string dir_path);
    ~Project();
    ID add_video(Video *vid);
    void remove_video(ID id);
    // read and write operator for Projects
    void read(const QJsonObject& json);
    void write(QJsonObject& json);

// TODO
//    void add_analysis();
//    void add_drawing();      
public:
    ID id;
    ID v_id;
    std::string name;
    ProjFiles* files;
    std::map<ID,Video*> videos;
    bool saved;
};


#endif // PROJECT_H
