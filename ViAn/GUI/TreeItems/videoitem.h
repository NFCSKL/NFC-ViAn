#ifndef VIDEOITEM_H
#define VIDEOITEM_H
#include "treeitem.h"

class VideoProject;
class VideoItem : public TreeItem {
    VideoProject* m_vid_proj = nullptr;
    const QString SEQUENCE_CONTAINER_NAME = "Images";
    const QIcon error_icon = QIcon(":/Icons/error_stop.png");
public:
    VideoItem(VideoProject* video_project);
    VideoItem();
    VideoProject* get_video_project();
    void set_video_project(VideoProject* vid_proj);
    void load_sequence_items();
    ~VideoItem();

    void remove();
    void rename();

    void set_thumbnail();
private:
    void load_thumbnail();
};
#endif // VIDEOITEM_H
