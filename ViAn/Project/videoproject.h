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
#include "Project/Analysis/analysismeta.h"
#include "Project/report.h"

/**
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class VideoProject : public Saveable{
    friend class VideoProjectTest;
    std::map<ID,Bookmark*> m_bookmarks;
    std::map<ID,AnalysisMeta*> m_analyses;
    Overlay* m_overlay = new Overlay();
    Video* video = nullptr;
    ID m_bm_cnt = 0;  // Bookmark id counter
    ID m_ana_cnt = 0; // Analysis id counter
public:
    VideoProject(Video* v); //Needs to have a video
    VideoProject();

    Q_DECL_DEPRECATED ID id;

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    ID add_analysis(AnalysisMeta* analysis);
    ID add_bookmark(Bookmark* bookmark);

    void delete_analysis(const int& id);
    void delete_bookmark(const int& id);
    void delete_artifacts();

    Video* get_video();
    Overlay* get_overlay();
    std::map<ID,Bookmark*> get_bookmarks();
    std::map<ID,AnalysisMeta*> get_analyses();
    AnalysisMeta *get_analysis(const int &id);
};


#endif // VIDEOPROJECT_H
