#ifndef VIDEOLISTITEM_H
#define VIDEOLISTITEM_H

#include <QListWidgetItem>

class VideoProject;

/**
 * @brief The VideoListItem class
 * Videoproject wrapper to store in QListWidget
 */
class VideoListItem : public QListWidgetItem {

public:
    VideoProject* m_vid_proj;
    VideoListItem(VideoProject* vid_proj);
};
#endif // VIDEOLISTITEM_H
