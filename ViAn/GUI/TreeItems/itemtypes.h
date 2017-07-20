#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include "Project/videoproject.h"
#include <QTreeWidgetItem>
#include "Project/Analysis/tag.h"
enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM, FOLDER_ITEM, TAG_ITEM};

class AnalysisItem : public QTreeWidgetItem {
    AnalysisProxy* m_analysis = nullptr;
    bool finished = false;
public:
    AnalysisItem(int type);
    ~AnalysisItem();
    void set_analysis(AnalysisProxy* analysis);
    AnalysisProxy *get_analysis();
    bool is_finished() const;
};

class TagItem : public QTreeWidgetItem {
    Tag* m_tag;
public:
    TagItem(Tag *tag, int type);
    Tag *get_tag();
};

class VideoItem : public QTreeWidgetItem {
    VideoProject* m_vid_proj;

public:
    VideoItem(VideoProject* video_project, int type);
    VideoProject* get_video_project();
    ~VideoItem();
signals:

public slots:
};

class FolderItem : public QTreeWidgetItem {

public:
    FolderItem(int type);
};
#endif // ITEMTYPES_H
