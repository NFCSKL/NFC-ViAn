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
#include "analysis.h"
/**
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class VideoProject{
    std::vector<Bookmark*> bookmarks;
    Video* video = nullptr;
    ID vid_id;


public:
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void add_bookmark(Bookmark* bookmark);
    ID add_analysis(Analysis analysis);
    void delete_artifacts();    
    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    Video* get_video();
    std::vector<Bookmark*> get_bookmarks();
    std::map<ID,Analysis> analyses;

};


#endif // VIDEOPROJECT_H
