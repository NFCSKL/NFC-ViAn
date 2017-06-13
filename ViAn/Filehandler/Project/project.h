#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "Filehandler/filehandler.h"
#include "videoproject.h"
#include "video.h"
#include "Filehandler/saveable.h"
#include "Filehandler/Analysis/analysis.h"

typedef int ID;
class FileHandler;

/**
 * @brief The Project class
 * incomplete class, will be added on
 * along with parser functionality
 */
class Project : public Saveable{
    FileHandler* file_handler;
    bool changes_made;
    std::map<ID,VideoProject*> videos;
    std::vector<Report*> reports;
public:
    std::string name;
    ID id;
    ID video_counter;
    ID dir;
    ID dir_bookmarks;
    ID dir_videos;

public:
    Project(FileHandler* file_handler);
    Project(FileHandler* file_handler, ID id, std::string name);
    ~Project();
    void add_report(Report* report);
    ID add_video(Video *vid);
    ID add_video_project(VideoProject* vid_proj);
    ID add_bookmark(ID video_counter, Bookmark *bookmark);
    ID add_analysis(ID video_counter, Analysis analysis);
    void add_report(std::string file_path);

    void delete_artifacts();
    void remove_video_project(ID id);
    // read and write operator for Projects
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void add_analysis(Analysis an);
    bool is_saved();
    void save_project();
    std::map<ID, VideoProject *>& get_videos();
    VideoProject* get_video(ID id);
    bool proj_equals(Project& other);
// TODO

//    void add_drawing();      
};


#endif // PROJECT_H
