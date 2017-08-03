#include "vidprojitem.h"

VidProjItem::VidProjItem(VideoProject *vid_proj)
{
    m_vid_proj = vid_proj;
    setText(QString::fromStdString(vid_proj->get_video()->file_path));
}
