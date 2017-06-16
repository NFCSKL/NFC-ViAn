#ifndef ITEMTYPES_H
#define ITEMTYPES_H
#include "Project/videoproject.h"
#include <QTreeWidgetItem>
enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM};

class AnalysisItem : public QTreeWidgetItem
{
public:
    AnalysisItem(int type);
};

class VideoItem : public QTreeWidgetItem
{
    VideoProject* m_vid_proj;

public:
    VideoItem(VideoProject* video_project, int type);
signals:

public slots:
};
#endif // ITEMTYPES_H
