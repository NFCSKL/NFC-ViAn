#ifndef PROJECT_H
#define PROJECT_H

#include "Filehandler/saveable.h"

#include <QJsonObject>
#include <QStandardPaths>

#include <vector>

class AnalysisProxy;
class Bookmark;
class BookmarkCategory;
class VideoInterval;
class VideoProject;

using ID = int;
/**
 * @brief The Project class
 * incomplete class, will be added on
 * along with parser functionality
 */
class Project : public Saveable{
    friend class ProjectTestsuite;

    QString m_name = "";            // Simply the project name
    QString m_dir_bookmarks = "";   // Project directory + /Bookmarks
    QString m_file = "";            // Full path to the project file: project path + project name + .vian
    QString last_changed = "";      // Date and time when the project was last saved

    std::vector<VideoProject*> m_videos;
    std::vector<Bookmark*> m_bookmarks;
    std::vector<BookmarkCategory*> m_categories;
    std::vector<AnalysisProxy*> m_analyses;
    std::vector<VideoInterval*> m_intervals;

    int m_vid_count = 0;
    int m_cat_count = 0;
    int m_interval_count = 0;
    bool m_temporary = true;
    bool m_unsaved_changes = false;
public:
    static Project* fromFile(const QString& file_name);
    Project(const QString &name, const QString &dir_path="");
    ~Project();

    QString m_dir = "";             // Path to the project folder: choosen path + project name

    ID add_video_project(VideoProject *vid_proj);
    void remove_video_project(VideoProject* vid_proj);

    void add_bookmark(Bookmark* bmark);
    void remove_bookmark(Bookmark* bmark);

    ID add_category(BookmarkCategory* cat);
    void remove_category(BookmarkCategory* cat);

    void add_analysis(AnalysisProxy* ana);
    void remove_analysis(AnalysisProxy* ana);

    void add_interval(VideoInterval* interval);
    void remove_interval(VideoInterval* interval);

    // read and write operator for Projects
    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    void set_unsaved(const bool& changed);
    void set_name_and_path(const QString& name, const QString& path);
    void set_temporary(const bool& is_temporary);

    bool is_saved() const;
    bool is_temporary() const;
    bool save_project();
    bool copy_directory_files(const QString &fromDir, const QString &toDir, const bool& coverFileIfExist, const std::vector<QString> &exclude_suffix);
    bool remove_files();
    QString generate_tmp_directory();

    std::vector<VideoProject *>& get_videos();
    std::vector<Bookmark *>& get_bookmarks();
    std::vector<BookmarkCategory *>& get_categories();
    std::vector<AnalysisProxy *>& get_analyses();
    std::vector<VideoInterval *> &get_intervals();
    VideoProject* get_video_project(int id);
    QString get_dir() const;
    QString get_name() const;
    QString get_file() const;
    QString get_last_changed() const;
    void set_name(QString);
    void set_dir(QString);
    void set_file(QString);
private:
    Project();
};


#endif // PROJECT_H
