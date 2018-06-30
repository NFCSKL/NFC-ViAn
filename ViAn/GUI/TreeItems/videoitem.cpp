#include "videoitem.h"

//const QString VideoItem::SEQUENCE_CONTAINER_NAME = "Image";

VideoItem::VideoItem(VideoProject* video_project): TreeItem(VIDEO_ITEM) {
    m_vid_proj = video_project;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));
    auto vid = m_vid_proj->get_video();
    if (vid && vid->is_sequence()) {
        load_sequence_items();
    }
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

/**
 * @brief VideoItem::load_sequence_items
 * Adds each image from the sequence as a child to the list item.
 */
void VideoItem::load_sequence_items() {
    if (m_vid_proj == nullptr ) return;
    auto seq = dynamic_cast<ImageSequence*>(m_vid_proj->get_video());
    if (seq) {
        QTreeWidgetItem* container = new QTreeWidgetItem();
        container->setText(0, SEQUENCE_CONTAINER_NAME);
        addChild(container);
        int i{};
        for (auto img_name : seq->get_image_names()) {
            container->addChild(new SequenceItem(img_name, i++));
        }
    }
}
