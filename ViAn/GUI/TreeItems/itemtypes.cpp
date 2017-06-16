#include "itemtypes.h"


VideoItem::VideoItem(VideoProject* video_project, int type): QTreeWidgetItem(type) {
    m_vid_proj = video_project;
}

AnalysisItem::AnalysisItem(int type) : QTreeWidgetItem(type) {

}
