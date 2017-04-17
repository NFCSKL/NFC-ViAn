#ifndef VIDEOPROJECT_H
#define VIDEOPROJECT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <QJsonArray>
#include <QFile>
#include "bookmark.h"
#include "video.h"
enum ANALYSIS_TYPE{MOVEMENT}; // Analyses have different types
/**
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class VideoProject{
    std::vector<Bookmark*> bookmarks;
    Video* video = nullptr;



public:
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void add_bookmark(Bookmark* bookmark);
    void delete_artifacts();    
    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    Video* get_video();
    std::vector<Bookmark*> get_bookmarks();

};


#endif // VIDEOPROJECT_H
