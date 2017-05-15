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
    Overlay* overlay = new Overlay();
    Video* video = nullptr;
    ID id_bookmark = 0;
    ID id_analysis = 0;


public:
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    ID add_analysis(Analysis analysis);
    ID add_bookmark(Bookmark* bookmark);
    void delete_artifacts();    
    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    Video* get_video();
    std::map<ID,Analysis> analyses;
    Overlay* get_overlay();
    std::map<ID,Bookmark*> get_bookmarks();

};


#endif // VIDEOPROJECT_H
