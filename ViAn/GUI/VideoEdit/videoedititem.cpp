#include "videoedititem.h"

#include "imagegenerator.h"
#include "Project/videoproject.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QPixmap>

VideoEditItem::VideoEditItem(int start, int end, VideoProject* vid_proj, QListWidget* parent) : QListWidgetItem(parent) {
    m_vid_proj = vid_proj;
    m_path = vid_proj->get_video()->file_path;
    m_start = start;
    m_end = end;
    m_last_frame = vid_proj->get_video()->get_last_frame();
    set_name();
    set_icon();
    setToolTip(m_vid_proj->get_video()->get_name());
}

void VideoEditItem::set_name() {
    QString name = QString::number(m_start) + " - " + QString::number(m_end);
    setText(name);
}

void VideoEditItem::set_icon() {
    std::string path = m_path.toStdString();
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) return;
    cv::Mat frame;
    cap >> frame;
    switch (frame.type()) {
    case CV_8UC1:
        cvtColor(frame, frame, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(frame, frame, CV_BGR2RGB);
        break;
    }

    ImageGenerator im_gen(frame, m_vid_proj->get_proj_path());
    QString thumbnail_path = im_gen.create_thumbnail(m_vid_proj->get_video()->get_name());
    const QIcon icon(thumbnail_path);
    setIcon(icon);
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

QString VideoEditItem::get_path() {
    return m_path;
}

void VideoEditItem::set_start(int new_start) {
    m_start = new_start;
    set_name();
}

void VideoEditItem::set_end(int new_end) {
    m_end = new_end;
    set_name();
}
