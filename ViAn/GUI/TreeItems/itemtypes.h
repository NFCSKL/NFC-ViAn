#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include "Project/videoproject.h"
#include <QTreeWidgetItem>

enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM, FOLDER_ITEM, TAG_ITEM};

class AnalysisItem : public QTreeWidgetItem
{
    Analysis m_analysis;

public:
    AnalysisItem(int type);
    ~AnalysisItem();
    void set_analysis(Analysis analysis);
    Analysis *get_analysis();
};

class TagItem : public QTreeWidgetItem
{
    Analysis m_tag;
public:
    TagItem(Analysis tag, int type);
    Analysis *get_tag();
};

class VideoItem : public QTreeWidgetItem
{
    VideoProject* m_vid_proj;

public:
    VideoItem(VideoProject* video_project, int type);
    VideoProject* get_video_project();
    ~VideoItem();
signals:

public slots:
};

class FolderItem : public QTreeWidgetItem
{
public:
    FolderItem(int type);
};
#endif // ITEMTYPES_H
