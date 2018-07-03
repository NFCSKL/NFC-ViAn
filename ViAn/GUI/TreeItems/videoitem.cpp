#include "videoitem.h"
#include "imagegenerator.h"

VideoItem::VideoItem(VideoProject* video_project): TreeItem(VIDEO_ITEM) {
    m_vid_proj = video_project;
    setFlags(flags() | Qt::ItemIsDragEnabled);
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));
    set_thumbnail();
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
    load_thumbnail();
}

/**
 * @brief VideoItem::set_thumbnail
 * Create a thumbnail from the video and set it as icon.
 */
void VideoItem::set_thumbnail() {
    std::string path = m_vid_proj->get_video()->file_path;
    cv::VideoCapture cap(path);
    cv::Mat frame;
    cap >> frame;
    ImageGenerator im_gen(frame, m_vid_proj->get_proj_path());
    std::string thumbnail_path = im_gen.create_thumbnail(m_vid_proj->get_video()->get_name());
    const QIcon icon(QString::fromStdString(thumbnail_path));
    setIcon(0, icon);
}

/**
 * @brief VideoItem::load_thumbnail
 * Loads the thumbnail path and sets it as icon
 */
void VideoItem::load_thumbnail() {
    std::string path = m_vid_proj->get_proj_path() + ImageGenerator::THUMBNAIL_FOLDER + m_vid_proj->get_video()->get_name() + ".png";
    const QIcon icon(QString::fromStdString(path));
    setIcon(0, icon);
}

