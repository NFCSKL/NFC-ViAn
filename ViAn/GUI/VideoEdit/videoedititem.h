#ifndef VIDEOEDITITEM_H
#define VIDEOEDITITEM_H

#include <QListWidgetItem>

class Project;
class VideoProject;
class VideoEditItem : public QListWidgetItem {
    Project* m_proj = nullptr;
    int m_vid_proj_id;
    QString m_path = "";
    int m_start = -1;
    int m_end = -1;
    int m_last_frame = -1;
    int m_index = -1;

    void set_name();
    void set_icon();
public:
    VideoEditItem(int start, int end, VideoProject* vid_proj, Project* proj, QListWidget *parent = nullptr);

    bool m_unsaved_changes = true;

    int get_vid_proj_id();
    void set_vid_proj_id(int new_id);

    int get_start();
    int get_end();
    int get_last_frame();
    int get_index();
    QString get_path();
    VideoProject* get_proj();
    void set_start(int new_start);
    void set_end(int new_end);
    void set_index(int index);

    bool is_saved() const;

    bool operator<(const VideoEditItem &other) const;
};

#endif // VIDEOEDITITEM_H
