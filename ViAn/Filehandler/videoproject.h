#ifndef VIDEOPROJECT_H
#define VIDEOPROJECT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <QJsonArray>
#include <QFile>
#include "Video/overlay.h"
#include "bookmark.h"
#include "video.h"
#include "analysis.h"
#include "report.h"

/**
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class VideoProject{
    std::map<ID,Bookmark*> bookmarks;
    std::map<ID,Analysis> analyses;
    Overlay* overlay = new Overlay();
    Video* video = nullptr;
    ID vid_id;
    ID id_bookmark = 0;

public:
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    ID add_analysis(Analysis analysis);
    ID add_bookmark(Bookmark* bookmark);
    void delete_artifacts();    
    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    Video* get_video();
    Overlay* get_overlay();
    std::map<ID,Bookmark*> get_bookmarks();
    std::map<ID,Analysis> get_analyses();
};


#endif // VIDEOPROJECT_H
