#include "itemcreator.h"

#include "imagegenerator.h"
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/poi.h"
#include "utility.h"
#include "yolowidgetitem.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <QDebug>

ItemCreator::ItemCreator(Analysis* analysis, std::vector<YoloWidgetItem*>* detection_list,
                         std::vector<int> frame_list, QString v_path, QString p_path) {
    m_analysis = analysis;
    m_detection_list = detection_list;
    m_frame_list = frame_list;
    video_path = v_path;
    proj_dir = p_path;
}

void ItemCreator::create_detection_items() {
    for (auto item : *m_detection_list) {
        delete item;
    }
    m_detection_list->clear();

    cv::Mat frame;
    cv::VideoCapture cap(video_path.toStdString());

    last_frame = int(cap.get(cv::CAP_PROP_FRAME_COUNT)) - 1;
    int progress = 0;
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

            ImageGenerator im_gen(dst, proj_dir);
            QString thumbnail_path = im_gen.create_thumbnail(Utility::name_from_path(video_path), 500);
            y_item->set_icon(thumbnail_path);
            m_detection_list->push_back(y_item);
            emit detection_added();
        }
        emit update_progress(++progress);
    }
    emit send_last_frame(last_frame);
    emit finished();
}
