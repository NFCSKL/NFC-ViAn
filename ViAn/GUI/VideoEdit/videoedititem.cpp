#include "videoedititem.h"

#include "constants.h"
#include "imagegenerator.h"
#include "Project/imagesequence.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "utility.h"
#include "Video/framemanipulator.h"
#include "videointerval.h"

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
    if (m_interval->get_vid_proj()->get_video()->is_sequence()) {
        ImageSequence* img_seq = dynamic_cast<ImageSequence*>(m_interval->get_vid_proj()->get_video());
        name = Utility::name_from_path(img_seq->get_search_path()) + "\n" + name;
    } else {
        name = Utility::name_from_path(m_path) + "\n" + name;
    }
    setText(name);
}

void VideoEditItem::set_icon() {
    std::string path = m_path.toStdString();
    cv::VideoCapture cap(path);
    if (!cap.isOpened()) return;
    cv::Mat frame;
    cap.set(CV_CAP_PROP_POS_FRAMES, get_start());
    cap >> frame;
    // Update color corrections
    FrameManipulator manipulator;
    manipulator.set_brightness(m_interval->get_state().brightness);
    manipulator.set_contrast(m_interval->get_state().contrast);
    manipulator.set_gamma(m_interval->get_state().gamma);
    manipulator.apply(frame);

    // Update rotation
    int rotation = m_interval->get_state().rotation;
    if (rotation == Constants::DEGREE_MIN) {
        rotation = -1;
    } else if (rotation == Constants::DEGREE_90) {
        rotation = cv::ROTATE_90_CLOCKWISE;
    } else if (rotation == Constants::DEGREE_180) {
        rotation = cv::ROTATE_180;
    } else if (rotation == Constants::DEGREE_270) {
        rotation = cv::ROTATE_90_COUNTERCLOCKWISE;
    }
    if (rotation != -1) cv::rotate(frame, frame, rotation);

    // Update flip
    int flip = 5;       // 5 means no flip
    bool flip_h = m_interval->get_state().flip_h;
    bool flip_v = m_interval->get_state().flip_v;
    if (rotation == cv::ROTATE_90_CLOCKWISE || rotation == cv::ROTATE_90_COUNTERCLOCKWISE) {
        std::swap(flip_h, flip_v);
    }
    if (flip_h && flip_v) {
        flip = -1;
    } else if (flip_h) {
        flip = 0;
    } else if (flip_v) {
        flip = 1;
    }
    if (flip != 5) cv::flip(frame, frame, flip);

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
