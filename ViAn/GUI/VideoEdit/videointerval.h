#ifndef VIDEOINTERVAL_H
#define VIDEOINTERVAL_H

#include "Filehandler/saveable.h"
#include "Project/video.h"

#include <QJsonObject>

class Project;
class VideoProject;
class VideoInterval : public Writeable {
    int m_vid_proj_id = -1;
    int m_start = -1;
    int m_end = -1;
    int m_index = -1;

    QString m_path = "";
    int m_last_frame = -1;
    Project* m_proj = nullptr;
    VideoState m_state;

    bool m_unsaved_changes = true;
public:
    VideoInterval(const int& start, const int& end, VideoProject* vid_proj);
    VideoInterval();

    int get_index();
    int get_start();
    int get_end();
    int get_last_frame();
    int get_proj_id();
    VideoProject* get_vid_proj();
    Project* get_proj();
    VideoState get_state();

    void set_start(int start);
    void set_end(int end);
    void set_index(int index);
    void set_project(Project* proj);

    bool is_saved() const;
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};

#endif // VIDEOINTERVAL_H
