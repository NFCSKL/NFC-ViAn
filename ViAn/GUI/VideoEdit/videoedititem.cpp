#include "videoedititem.h"

#include "imagegenerator.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "utility.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QPixmap>
#include <QDebug>

VideoEditItem::VideoEditItem(int start, int end, VideoProject* vid_proj, Project* proj, QListWidget* parent) : QListWidgetItem(parent) {
    m_proj = proj;
    m_vid_proj_id = vid_proj->get_id();
    m_path = vid_proj->get_video()->file_path;
    m_start = start;
    m_end = end;
    m_last_frame = vid_proj->get_video()->get_last_frame();
    set_name();
    set_icon();
    setToolTip(vid_proj->get_video()->get_name());
}

void VideoEditItem::set_name() {
    QString name = QString::number(m_start) + " - " + QString::number(m_end);
    name = Utility::name_from_path(m_path) + "\n" + name;
    setText(name);
}

void VideoEditItem::set_icon() {
    std::string path = m_path.toStdString();
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) return;
    cv::Mat frame;
    cap.set(CV_CAP_PROP_POS_FRAMES, m_start);
    cap >> frame;
    switch (frame.type()) {
    case CV_8UC1:
        cvtColor(frame, frame, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(frame, frame, CV_BGR2RGB);
        break;
    }

    ImageGenerator im_gen(frame, m_proj->get_dir());
    QString thumbnail_path = im_gen.create_thumbnail(Utility::name_from_path(m_path));

    const QIcon icon(thumbnail_path);
    setIcon(icon);
}

int VideoEditItem::get_vid_proj_id() {
    return m_vid_proj_id;
}

void VideoEditItem::set_vid_proj_id(int new_id) {
    m_vid_proj_id = new_id;
}

int VideoEditItem::get_start() {
    return m_start;
}

int VideoEditItem::get_end() {
    return m_end;
}

int VideoEditItem::get_last_frame() {
    return m_last_frame;
}

int VideoEditItem::get_index() {
    return m_index;
}

QString VideoEditItem::get_path() {
    return m_path;
}

VideoProject* VideoEditItem::get_proj() {
    return m_proj->get_video_project(m_vid_proj_id);
}

void VideoEditItem::set_start(int new_start) {
    m_start = new_start;
    set_name();
}

void VideoEditItem::set_end(int new_end) {
    m_end = new_end;
    set_name();
}

void VideoEditItem::set_index(int index) {
    m_index = index;
}

bool VideoEditItem::is_saved() const{
    return !m_unsaved_changes;
}

bool VideoEditItem::operator<(const QListWidgetItem& other) const {
    int index1, index2;

    VideoEditItem* ve_item1 = dynamic_cast<VideoEditItem*>(const_cast<QListWidgetItem*>(&other));
    index1 = const_cast<VideoEditItem*>(this)->get_index();
    index2 = ve_item1->get_index();

    bool res = index1 < index2;
    return res;
}
