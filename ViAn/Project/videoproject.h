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
#include "Project/Analysis/analysisproxy.h" // TODO Include basic instead
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
    Video* m_video = nullptr;
    Project* m_project = nullptr;
    ID m_bm_cnt = 0;  // Bookmark id counter
    ID m_ana_id = 0; // Analysis id counter

    bool m_unsaved_changes = true;
    bool current = false;

public:

    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    ~VideoProject();

    VideoState state;

    Q_DECL_DEPRECATED ID id;

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    ID add_analysis(BasicAnalysis* analysis);
    ID add_bookmark(Bookmark* bookmark);

    void remove_analysis(BasicAnalysis* analysis);
    void remove_bookmark(Bookmark* bookmark);

    void set_tree_index(std::stack<int> tree_index);
    void set_project(Project* proj);

    // used to update path if project was moved and opened
    void reset_root_dir(const std::string& dir);

    void delete_analysis(const int& id);
    void delete_bookmark(const int& id);
    void delete_artifacts();

    // TODO, not used
    void remove_from_project();

    std::string get_index_path();
    std::string get_proj_path();
    Video* get_video();
    Overlay* get_overlay();
    std::map<ID,Bookmark*> get_bookmarks();
    std::map<ID,BasicAnalysis*> get_analyses();
    BasicAnalysis *get_analysis(const int &id);

    bool is_saved();
    bool is_current();
    void set_current(bool);
};


#endif // VIDEOPROJECT_H
