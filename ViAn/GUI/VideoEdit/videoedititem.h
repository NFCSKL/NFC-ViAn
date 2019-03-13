#ifndef VIDEOEDITITEM_H
#define VIDEOEDITITEM_H

#include <QListWidgetItem>

class VideoProject;
class VideoEditItem : public QListWidgetItem {
    VideoProject* m_vid_proj = nullptr;
    QString m_path = "";
    int m_start = -1;
    int m_end = -1;
    int m_last_frame = -1;

    void set_name();
    void set_icon();
public:
    VideoEditItem(int start, int end, VideoProject* vid_proj, QListWidget *parent = nullptr);

    int get_start();
    int get_end();
    int get_last_frame();
    QString get_path();
    void set_start(int new_start);
    void set_end(int new_end);
};

#endif // VIDEOEDITITEM_H
