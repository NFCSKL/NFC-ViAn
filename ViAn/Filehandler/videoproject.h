#ifndef VIDEOPROJECT_H
#define VIDEOPROJECT_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <QJsonArray>
#include <QFile>
#include "analysis.h"
#include "bookmark.h"
enum ANALYSIS_TYPE{MOVEMENT}; // Analyses have different types
/**
 * @todo change map to include ID for flexibility.. I.E allow multiple analyses of each type.
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class VideoProject{
    std::map<ANALYSIS_TYPE, Analysis> analyses; // Only one analysis of each type
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

};


#endif // VIDEOPROJECT_H
