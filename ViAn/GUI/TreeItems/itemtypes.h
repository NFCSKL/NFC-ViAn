#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include "Project/videoproject.h"
#include <QTreeWidgetItem>

enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM, FOLDER_ITEM};

class AnalysisItem : public QTreeWidgetItem
{
    Analysis m_analysis;

public:
    AnalysisItem(int type);
    void set_analysis(Analysis analysis);
    Analysis *get_analysis();
};

class FolderItem : public QTreeWidgetItem
{
public:
    FolderItem(int type);
};

class VideoItem : public QTreeWidgetItem
{
    VideoProject* m_vid_proj;

public:
    VideoItem(VideoProject* video_project, int type);
    VideoProject* get_video_project();
signals:

public slots:
};
#endif // ITEMTYPES_H
