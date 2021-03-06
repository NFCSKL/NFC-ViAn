#include "videolistitem.h"

#include "Project/videoproject.h"

/**
 * @brief VideoListItem::VideoListItem
 * @param vid_proj
 * Set vid proj and text
 */
VideoListItem::VideoListItem(VideoProject *vid_proj) {
    m_vid_proj = vid_proj;
    setText(m_vid_proj->get_video()->get_name());
}
