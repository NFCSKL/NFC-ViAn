#ifndef PROJECT_H
#define PROJECT_H
#include <map>
#include <string>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
//QT
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
// vian
#include "videoproject.h"
#include "video.h"
#include "Filehandler/saveable.h"
#include "Project/Analysis/analysismeta.h"

using ID = int;
/**
 * @brief The Project class
 * incomplete class, will be added on
 * along with parser functionality
 */
class Project : public Saveable{
    friend class ProjectTestsuite;

    std::string m_name = "";
    std::string m_dir = "";
    std::string m_dir_bookmarks = "";
    std::string m_dir_videos = "";
    Q_DECL_DEPRECATED bool m_changes = true;
    std::map<ID,VideoProject*> m_videos;
    std::vector<Report*> m_reports;
    ID m_vid_count = 0;
public:  
    Q_DECL_DEPRECATED int m_id = -1;

public:
    static Project* fromFile(const std::string& full_path);
    Project(const std::string& name, const std::string& dir_path);
    ~Project();

    ID add_report(Report* report);
    ID add_video_project(VideoProject *vid_proj);
    void remove_video_project(const int &id);
    void remove_report(const int& id);

    Q_DECL_DEPRECATED ID add_bookmark(const int& vid_id,  Bookmark *bookmark);
    Q_DECL_DEPRECATED ID add_analysis(const int& vid_id,  AnalysisMeta& analysis);




    // read and write operator for Projects
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void delete_artifacts();

    bool is_saved() const;
    bool save_project();
    bool load_project();
    std::map<ID, VideoProject *>& get_videos();
    VideoProject* get_video(ID m_id);
// TODO

//    void add_drawing();      
    std::string getDir_bookmarks() const;
    std::string getDir_videos() const;
    std::string getDir() const;
    std::string getName() const;
private:
    Project();
};


#endif // PROJECT_H
