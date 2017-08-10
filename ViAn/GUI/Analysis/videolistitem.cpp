#include "videolistitem.h"

/**
 * @brief VideoListItem::VideoListItem
 * @param vid_proj
 * Set vid proj and text
 */
VideoListItem::VideoListItem(VideoProject *vid_proj)
{        
    m_vid_proj =vid_proj;
    setText(QString::fromStdString(m_vid_proj->get_video()->get_name()));    
}

/**
 * @brief AnalysisListItem::AnalysisListItem
 * @param method
 * Set method and text
 */
AnalysisListItem::AnalysisListItem(AnalysisMethod *method)
{
    m_analysis = method;
    setText(QString::fromStdString("method"));
}
