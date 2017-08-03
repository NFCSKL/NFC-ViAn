#ifndef VIDEOLISTITEM_H
#define VIDEOLISTITEM_H
#include <QListWidgetItem>
#include <QLayout>
#include "Project/videoproject.h"
#include "Analysis/analysismethod.h"
class VideoListItem : public QListWidgetItem
{    

public:
    VideoProject* m_vid_proj;
    VideoListItem(VideoProject* vid_proj);
};

class AnalysisListItem : public QListWidgetItem{

public:
    AnalysisMethod* m_analysis;
    AnalysisListItem(AnalysisMethod* method);
};


#endif // VIDEOLISTITEM_H
