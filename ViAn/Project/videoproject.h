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
#include "project.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/report.h"
#include <stack>

/**
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class Project;
class Bookmark;
class VideoProject : public Saveable{
    friend class VideoProjectTest;
    std::map<ID,Bookmark*> m_bookmarks;
    std::string m_tree_index = "";
    std::map<ID,BasicAnalysis*> m_analyses;
    Overlay* m_overlay = new Overlay();
    Video* video = nullptr;
    Project* m_project = nullptr;
    ID m_bm_cnt = 0;  // Bookmark id counter
    ID m_ana_cnt = 0; // Analysis id counter
public:

    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    ~VideoProject();

    Q_DECL_DEPRECATED ID id;

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    ID add_analysis(BasicAnalysis* analysis);
    ID add_bookmark(Bookmark* bookmark);

    void set_tree_index(std::stack<int> tree_index);
    void set_project(Project* proj);

    // used to update path if project was moved and opened
    void reset_root_dir(const std::string& dir);

    void delete_analysis(const int& id);
    void delete_bookmark(const int& id);
    void delete_artifacts();

    std::string get_index_path();
    Video* get_video();
    Overlay* get_overlay();
    std::map<ID,Bookmark*> get_bookmarks();
    std::map<ID,BasicAnalysis*> get_analyses();
    BasicAnalysis *get_analysis(const int &id);

};


#endif // VIDEOPROJECT_H
