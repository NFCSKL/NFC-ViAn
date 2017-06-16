#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "Filehandler/filehandler.h"

class VideoItem : public QTreeWidgetItem
{
    VideoProject* m_vid_proj;
public:
    VideoItem(VideoProject* video_project);

signals:

public slots:
};

#endif // VIDEOITEM_H
