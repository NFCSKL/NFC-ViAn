#include "itemtypes.h"

TreeItem::TreeItem(int type) : QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}



VideoItem::VideoItem(VideoProject* video_project): TreeItem(VIDEO_ITEM) {
    m_vid_proj = video_project;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));
}

VideoItem::VideoItem(): TreeItem(VIDEO_ITEM) {
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0,"placeholder");
}

VideoItem::~VideoItem(){
    m_vid_proj->remove_from_project();
    delete m_vid_proj;
}

void VideoItem::remove(){
    qDebug() << "Remove VideoItem";

}

void VideoItem::rename(){}

VideoProject* VideoItem::get_video_project() {
    return m_vid_proj;
}

void VideoItem::set_video_project(VideoProject *vid_proj) {
    m_vid_proj = vid_proj;
    setText(0, QString::fromStdString(vid_proj->get_video()->get_name()));
}

AnalysisItem::AnalysisItem(AnalysisProxy* analysis) : TreeItem(ANALYSIS_ITEM) {
    m_analysis = analysis;
    finished = true;
    const QIcon folder_icon("../ViAn/Icons/analysis.png");
    setIcon(0, folder_icon);
    setText(0, QString::fromStdString(m_analysis->get_name()));
}

AnalysisItem::AnalysisItem() : TreeItem(ANALYSIS_ITEM) {
    const QIcon folder_icon("../ViAn/Icons/analysis.png");
    setIcon(0, folder_icon);
    setText(0, "Analysis");
}

AnalysisItem::~AnalysisItem() {}

void AnalysisItem::set_analysis(AnalysisProxy *analysis) {
    m_analysis = analysis;
    finished = true;
}

bool AnalysisItem::is_finished() const {
    return finished;
}

AnalysisProxy* AnalysisItem::get_analysis() {
    return m_analysis;
}

void AnalysisItem::remove(){}

void AnalysisItem::rename(){
//    m_analysis->set_name(text(0).toStdString());
}

FolderItem::FolderItem() : TreeItem(FOLDER_ITEM) {
    setFlags(flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    const QIcon folder_icon("../ViAn/Icons/folder.png");
    setIcon(0, folder_icon);
}

void FolderItem::remove() {}

void FolderItem::rename(){}

TagItem::TagItem(Tag *tag) : TreeItem(TAG_ITEM) {
    m_tag = tag;
    setText(0, QString::fromStdString(tag->get_name()));
    const QIcon folder_icon("../ViAn/Icons/tag.png");
    setIcon(0, folder_icon);
}

void TagItem::remove(){}

void TagItem::rename() {
//    m_tag->set_name(text(0).toStdString());
}

Tag *TagItem::get_tag() {
    return m_tag;
}


