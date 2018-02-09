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
    std::string m_name = "";
    std::string m_path = ""; // Path to where project folder is saved
    std::string m_dir = "";  //Path to the project temp folder
    std::string m_dir_bookmarks = "";
    std::string m_tmp_path = "";  // Path to the project file's temporary folder
    std::string m_save_path = ""; // Path to the project file where it is saved.

    QTemporaryDir tmp_dir;
    std::vector<VideoProject*> m_videos;
    std::map<ID, Report*> m_reports;
    int m_vid_count = 0;
    int m_rp_count = 0;
public:
    static Project* fromFile(const std::string& file_name);
    Project(const std::string& name, const std::string& dir_path);
    ~Project();

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
    void delete_artifacts();

    bool is_saved() const;
    bool save_project();
    bool save_project_from_tmp();
    bool copy_directory_files(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    bool load_project();

    void recurse_add_dir(QDir d, QStringList & list);

    std::vector<VideoProject *>& get_videos();
    VideoProject* get_video(const int& v_pos);
    std::string getDir_bookmarks() const;
    std::string getDir() const;
    std::string getName() const;
    std::string get_path() const;
    std::string get_save_path() const;

private:
    Project();
};


#endif // PROJECT_H
