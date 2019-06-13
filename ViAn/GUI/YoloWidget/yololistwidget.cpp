#include "yololistwidget.h"

#include "GUI/YoloWidget/yolowidgetitem.h"
#include "imagegenerator.h"
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/poi.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "utility.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QMouseEvent>

#include <QDebug>

YoloListWidget::YoloListWidget(QWidget *parent) : QListWidget(parent) {
    setMinimumSize(1000, 800);
    setViewMode(QListWidget::IconMode);
    setResizeMode(QListWidget::Adjust);
}

void YoloListWidget::set_analysis(AnalysisProxy* analysis) {
    if (!analysis) return;
    m_frame_list.clear();
    m_proxy = analysis;
    m_analysis = m_proxy->load_analysis();

    // Iterate over all intervals
    for (auto interval : m_analysis->get_intervals()) {
        int frame_nr = interval->get_start();
        while(interval->in_interval(frame_nr)) {
            m_frame_list.push_back(frame_nr);
            frame_nr += m_proxy->get_sample_freq();
        }
    }
    // Update the Combobox with the frames from the analysid
    emit update_frames(m_frame_list);
    create_detection_items();
    filter_detections();
    update_slider();
}

void YoloListWidget::create_detection_items() {
    m_detection_list.clear();

    QString path = m_proxy->get_video_path();
    cv::Mat frame;
    cv::VideoCapture cap(path.toStdString());

    last_frame = int(cap.get(cv::CAP_PROP_FRAME_COUNT)) - 1;

    // Iterate over all frames
    for (int frame_num : m_frame_list) {
        cap.set(cv::CAP_PROP_POS_FRAMES, frame_num);
        cap >> frame;

        switch (frame.type()) {
        case CV_8UC1:
            cvtColor(frame, frame, cv::COLOR_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            break;
        }

        // Iterate over all detection on the frame
        std::vector<DetectionBox> detections = m_analysis->get_detectionbox_on_frame(frame_num);
        for (DetectionBox d_box : detections) {
            YoloWidgetItem* y_item = new YoloWidgetItem();
            switch (m_analysis->type) {
            case MOTION_DETECTION:
                y_item->setText(QString::number(frame_num));
                break;
            case OBJECT_DETECTION: {
                QString text = QString::number(frame_num) + ": " +
                        QString::fromStdString(d_box.get_class_name()) + " " +
                        QString::number(int(d_box.get_confidence()*100))+"%";
                y_item->setText(text);
                y_item->class_name = QString::fromStdString(d_box.get_class_name());
                y_item->confidence = double(d_box.get_confidence());
                break;
            }
            default:
                break;
            }
            y_item->frame_nr = frame_num;
            cv::Rect rect = d_box.get_rect();   // printing rect here
            cv::Mat dst;
            try {
                cv::resize(frame(rect), dst, cv::Size(rect.width, rect.height));
            } catch (cv::Exception& e) {
                qWarning() << e.what();
                continue;
            }
            y_item->rect = rect;

            ImageGenerator im_gen(dst, m_proj->get_dir());
            QString thumbnail_path = im_gen.create_thumbnail(Utility::name_from_path(path), 500);
            const QIcon icon(thumbnail_path);
            y_item->setIcon(icon);
            m_detection_list.push_back(y_item);
        }
    }
}

/**
 * @brief YoloListWidget::update_video_widget
 * Update video widget in the mainwindow
 * @param frame
 */
void YoloListWidget::update_video_widget(int frame) {
    VideoProject* vid_proj = m_proj->get_video_project(m_proxy->get_vid_proj_id());
    int id = m_proxy->get_id();
    VideoState state;
    if (frame < 0) {
        state.frame = 0;
    } else {
        state.frame = frame;
    }
    vid_proj->get_video()->state = state;
    emit set_frame(vid_proj, id);
}

void YoloListWidget::filter_detections() {
    while (count()>0) {
        takeItem(0);
    }

    for (YoloWidgetItem* y_item : m_detection_list) {
        bool class_bool = false;
        bool frame_bool = false;
        bool conf_bool = false;

        // Check class
        if (m_class_name == "ALL") {
            class_bool = true;
        } else if (m_class_name == y_item->class_name) {
            class_bool = true;
        }
        // Check frame
        if (m_frame == -1) {
            frame_bool = true;
        } else if (m_frame == y_item->frame_nr) {
            frame_bool = true;
        }
        // Check confidence
        if (y_item->confidence >= m_confidence) {
            conf_bool = true;
        }

        if (class_bool && frame_bool && conf_bool) {
            addItem(y_item);
        }
    }
}

void YoloListWidget::update_frame_filter(QString text) {
    bool ok;
    int num = text.toInt(&ok);
    if (!ok) num = -1;
    m_frame = num;
    filter_detections();
    emit set_slider(num);
}

void YoloListWidget::update_confidence_filter(int confidence) {
    m_confidence = double(confidence)/100;
    filter_detections();
}

void YoloListWidget::update_class_filter(QString class_name) {
    m_class_name = class_name;
    filter_detections();
}

void YoloListWidget::update_slider() {
    emit slider_max(last_frame);
}

void YoloListWidget::set_project(Project* proj) {
    m_proj = proj;
}

void YoloListWidget::mousePressEvent(QMouseEvent* event) {
    if (itemAt(event->pos())) {
        switch (event->button()) {
        case Qt::LeftButton: {
            setCurrentItem(itemAt(event->pos()));
            break;
        }
        case Qt::RightButton:


            break;
        default:
            break;
        }
    }
}

void YoloListWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (itemAt(event->pos())) {
        YoloWidgetItem* yi_item = dynamic_cast<YoloWidgetItem*>(itemAt(event->pos()));
        update_video_widget(yi_item->frame_nr);
    }
}
