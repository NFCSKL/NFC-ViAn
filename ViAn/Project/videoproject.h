#ifndef VIDEOPROJECT_H
#define VIDEOPROJECT_H

#include "Filehandler/saveable.h"
#include "video.h"
#include "Video/overlay.h"

#include <QJsonObject>

#include <map>
#include <stack>

using ID = int;

/**
 * @brief The VideoProject class
 * Class for storing video and all its belonging components
 * such as analyses, drawings and documentation.
 */
class AnalysisProxy;
class BasicAnalysis;
class Bookmark;
class Project;
class Tag;
class Video;
class VideoProject : public Saveable {
    friend class VideoProjectTest;
    std::string m_tree_index = "";
    std::map<ID,BasicAnalysis*> m_analyses;
    Overlay* m_overlay = new Overlay();
    Video* m_video = nullptr;
    Project* m_project = nullptr;
    ID m_ana_id = 0; // Analysis id counter

    bool m_unsaved_changes = true;
    bool current = false;
    bool generated_video = false;

    ID id = -1;
public:

    VideoProject(Video* v); //Needs to have a video
    VideoProject();
    ~VideoProject();

    Tag* tag_seq_tag = nullptr;
    std::pair<int, int> m_interval = std::make_pair(-1, -1);

    VideoState state;

    int get_id() const;
    void set_id(const int& new_id);

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    ID add_analysis(BasicAnalysis* analysis);
    void remove_analysis(BasicAnalysis* analysis);

    void set_tree_index(std::stack<int> tree_index);
    void set_project(Project* proj);

    // used to update path if project was moved and opened
    void reset_root_dir(const QString &dir);

    std::string get_index_path();
    QString get_proj_path();
    Video* get_video();
    Overlay* get_overlay();
    std::map<ID,BasicAnalysis*> get_analyses();
    BasicAnalysis *get_analysis(const int &id);

    bool is_saved();
    bool is_current();
    void set_current(bool);
    bool is_generated_video();
    void set_generated_video(bool);
};

#endif // VIDEOPROJECT_H
