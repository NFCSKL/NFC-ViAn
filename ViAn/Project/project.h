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
    std::string m_dir = "";  // Path to the project folder --- c:/name/
    std::string m_tmp_dir = "";  //Path to the project temp folder --- /temp/ViAn-XXXXXX/name/
    std::string m_dir_bookmarks = "";
    std::string m_tmp_file = "";  // Path to the project file's temporary folder --- /temp/ViAn-XXXXXX/name/name.vian
    std::string m_file = ""; // Path to the project file where it is saved. --- /name/name.vian

    QTemporaryDir tmp_dir;
    std::vector<VideoProject*> m_videos;
    std::map<ID, Report*> m_reports;
    int m_vid_count = 0;
    int m_rp_count = 0;
    bool m_unsaved_changes = true;
public:
    static Project* fromFile(const std::string& file_name);
    Project(const std::string& name, const std::string& dir_path);
    ~Project();

    bool tmp_dir_valid = false;

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

    void set_unsaved(const bool& changed);

    bool is_saved() const;
    bool save_project();
    bool move_project_from_tmp();
    bool copy_directory_files(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    bool load_project();

    std::vector<VideoProject *>& get_videos();
    VideoProject* get_video(const int& v_pos);
    std::string getDir_bookmarks() const;
    std::string get_dir() const;
    std::string get_tmp_dir() const;
    std::string get_name() const;
    std::string get_file() const;

private:
    Project();
};


#endif // PROJECT_H
