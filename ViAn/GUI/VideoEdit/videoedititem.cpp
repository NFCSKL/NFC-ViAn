#include "videoedititem.h"

#include "imagegenerator.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "videointerval.h"
#include "utility.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QDebug>

VideoEditItem::VideoEditItem(VideoInterval* interval, QListWidget* parent) : QListWidgetItem(parent) {
    m_interval = interval;
    m_path = m_interval->get_vid_proj()->get_video()->file_path;
    set_name();
    set_icon();
    setToolTip(m_interval->get_vid_proj()->get_video()->get_name());
}

VideoEditItem::~VideoEditItem() {
}

void VideoEditItem::set_name() {
    QString name = QString::number(get_start()) + " - " + QString::number(get_end());
    name = Utility::name_from_path(m_path) + "\n" + name;
    setText(name);
}

void VideoEditItem::set_icon() {
    std::string path = m_path.toStdString();
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) return;
    cv::Mat frame;
    cap.set(CV_CAP_PROP_POS_FRAMES, get_start());
    cap >> frame;
    switch (frame.type()) {
    case CV_8UC1:
        cvtColor(frame, frame, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(frame, frame, CV_BGR2RGB);
        break;
    }

    ImageGenerator im_gen(frame, m_interval->get_proj()->get_dir());
    QString thumbnail_path = im_gen.create_thumbnail(Utility::name_from_path(m_path));

    const QIcon icon(thumbnail_path);
    setIcon(icon);
}

int VideoEditItem::get_index() {
    return m_interval->get_index();
}

int VideoEditItem::get_start() {
    return m_interval->get_start();
}

int VideoEditItem::get_end() {
    return m_interval->get_end();
}

int VideoEditItem::get_last_frame() {
    return m_interval->get_last_frame();
}

QString VideoEditItem::get_path() {
    return m_path;
}

VideoInterval* VideoEditItem::get_interval() {
    return m_interval;
}

void VideoEditItem::set_start(int new_start) {
    m_interval->set_start(new_start);
    set_name();
}

void VideoEditItem::set_end(int new_end) {
    m_interval->set_end(new_end);
    set_name();
}

bool VideoEditItem::operator<(const QListWidgetItem& other) const {
    int index1, index2;

    VideoEditItem* ve_item1 = dynamic_cast<VideoEditItem*>(const_cast<QListWidgetItem*>(&other));
    index1 = const_cast<VideoEditItem*>(this)->get_index();
    index2 = ve_item1->get_index();

    bool res = index1 < index2;
    return res;
}
