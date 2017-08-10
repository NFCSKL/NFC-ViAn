#ifndef VIDEOLISTITEM_H
#define VIDEOLISTITEM_H
#include <QListWidgetItem>
#include <QLayout>
#include "Project/videoproject.h"
#include "Analysis/analysismethod.h"

/**
 * @brief The VideoListItem class
 * Videoproject wrapper to store in QListWidget
 */
class VideoListItem : public QListWidgetItem
{    

public:
    VideoProject* m_vid_proj;
    VideoListItem(VideoProject* vid_proj);
};

/**
 * @brief The AnalysisListItem class
 * AnalysisItem wrapper for analysismethod
 * in Qlistwidget
 */
class AnalysisListItem : public QListWidgetItem{

public:
    AnalysisMethod* m_analysis;
    AnalysisListItem(AnalysisMethod* method);
};


#endif // VIDEOLISTITEM_H
