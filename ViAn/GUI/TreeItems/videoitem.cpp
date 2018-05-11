#include "videoitem.h"

VideoItem::VideoItem(VideoProject* video_project): TreeItem(VIDEO_ITEM) {
    m_vid_proj = video_project;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));
    load_sequence_items();
}

VideoItem::VideoItem(): TreeItem(VIDEO_ITEM) {
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0,"placeholder");
}

VideoItem::~VideoItem(){}

void VideoItem::remove(){}

void VideoItem::rename(){}

VideoProject* VideoItem::get_video_project() {
    return m_vid_proj;
}

void VideoItem::set_video_project(VideoProject *vid_proj) {
    m_vid_proj = vid_proj;
    setText(0, QString::fromStdString(vid_proj->get_video()->get_name()));
}

void VideoItem::load_sequence_items() {
    if (m_vid_proj == nullptr ) return;
    Video* vid = m_vid_proj->get_video();
    if (vid->is_sequence()) {
        auto seq = dynamic_cast<ImageSequence*>(vid);
        int i{};
        qDebug() << QString::fromStdString(seq->get_pattern_name());
        for (auto img_name : seq->get_image_names()) {
            this->addChild(new SequenceItem(img_name, i++));
        }
    }

}

