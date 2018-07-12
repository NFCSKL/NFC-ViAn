#ifndef VIDEOITEM_H
#define VIDEOITEM_H
#include "treeitem.h"
#include "Project/videoproject.h"
class VideoProject;
class VideoItem : public TreeItem {
    VideoProject* m_vid_proj = nullptr;
public:
    VideoItem(VideoProject* video_project);
    VideoItem();
    VideoProject* get_video_project();
    void set_video_project(VideoProject* vid_proj);
    ~VideoItem();

    void remove();
    void rename();

private:
    void set_thumbnail();
    void load_thumbnail();
};
#endif // VIDEOITEM_H
