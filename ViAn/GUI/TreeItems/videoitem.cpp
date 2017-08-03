#include "videoitem.h"

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

