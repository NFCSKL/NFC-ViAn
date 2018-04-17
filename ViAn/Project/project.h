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
#include <QFile>
#include <QTemporaryDir>
// vian
#include "videoproject.h"
#include "video.h"
#include "Filehandler/saveable.h"
#include "Project/projecttreestate.h"
#include "Project/Analysis/analysisproxy.h"


using ID = int;
/**
 * @brief The Project class
 * incomplete class, will be added on
 * along with parser functionality
 */
class Report;
class Project : public Saveable{
    friend class ProjectTestsuite;
    std::string m_name = "";            // Simply the project name
    std::string m_dir_bookmarks = "";   // Project directory + /Bookmarks
    std::string m_file = "";            // Full path to the project file: project path + project name + .vian

    std::vector<VideoProject*> m_videos;
    std::map<ID, Report*> m_reports;
    int m_vid_count = 0;
    int m_rp_count = 0;
    bool m_temporary = true;
    bool m_unsaved_changes = false;
public:
    static Project* fromFile(const std::string& file_name);
    Project(const std::string& name, const std::string& dir_path="");
    ~Project();

    std::string m_dir = "";             // Path to the project folder: choosen path + project name

    ID add_report(Report* report);
    ID add_video_project(VideoProject *vid_proj);
    // TODO
    //ID add_drawing();
    // void remove_drawing();
    void remove_video_project(VideoProject* vid_proj);
    void remove_report(const int& id);

    // read and write operator for Projects
    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    void set_unsaved(const bool& changed);
    void set_name_and_path(const std::string& name, const std::string& path);
    void set_temporary(const bool& is_temporary);

    bool is_saved() const;
    bool is_temporary() const;
    bool save_project();
    bool copy_directory_files(const QString &fromDir, const QString &toDir, const bool& coverFileIfExist, const std::vector<std::string>& exclude_suffix);
    bool remove_files();
    std::string generate_tmp_directory();

    std::vector<VideoProject *>& get_videos();
    VideoProject* get_video(const int& v_pos);
    std::string getDir_bookmarks() const;
    std::string get_dir() const;
    std::string get_name() const;
    std::string get_file() const;
    void set_name(std::string);
    void set_dir(std::string);
    void set_file(std::string);
private:
    Project();
};


#endif // PROJECT_H
