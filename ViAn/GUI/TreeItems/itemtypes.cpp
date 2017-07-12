#include "itemtypes.h"


VideoItem::VideoItem(VideoProject* video_project, int type): QTreeWidgetItem(type) {
    m_vid_proj = video_project;
}

VideoItem::~VideoItem(){
    // VideoProject should be deleted by the Project
}

VideoProject* VideoItem::get_video_project() {
    return m_vid_proj;
}

AnalysisItem::AnalysisItem(int type) : QTreeWidgetItem(type) {

}

AnalysisItem::~AnalysisItem() {
}

void AnalysisItem::set_analysis(Analysis analysis) {
    m_analysis = analysis;
}

Analysis* AnalysisItem::get_analysis() {
    return &m_analysis;
}

TagItem::TagItem(Analysis tag, int type) : QTreeWidgetItem(type) {
    m_tag = tag;
}

Analysis* TagItem::get_tag() {
    return &m_tag;
}

FolderItem::FolderItem(int type) : QTreeWidgetItem(type) {

}
