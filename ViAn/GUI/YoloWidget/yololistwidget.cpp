#include "yololistwidget.h"

#include "GUI/YoloWidget/yolowidgetitem.h"
#include "imagegenerator.h"
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "utility.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"

#include <QDebug>

YoloListWidget::YoloListWidget(QWidget *parent) : QListWidget(parent) {
    setMinimumSize(1000, 1000);
    setViewMode(QListWidget::IconMode);
    setResizeMode(QListWidget::Adjust);
}

void YoloListWidget::set_analysis(AnalysisProxy* analysis) {
    m_frame_list.clear();
    m_analysis = analysis;

    // Iterate over all intervals
    for (auto interval : m_analysis->get_intervals()) {
        int frame_nr = interval->get_start();
        while(interval->in_interval(frame_nr)) {
            qDebug() << "frame nr" << frame_nr;
            m_frame_list.push_back(frame_nr);
            frame_nr += ANA_FRAME_STEP;
        }
    }
    emit update_frames(m_frame_list);
    choose_ana_frame("New analysis");
}

void YoloListWidget::choose_ana_frame(QString text) {
    clear();
    bool ok;
    int num = text.toInt(&ok);
    if (!ok) return;
    show_frame(num);
    emit set_frame(num);
}

void YoloListWidget::show_frame(int frame_num) {
    QString path = m_analysis->get_video_path();
    cv::Mat frame;
    cv::VideoCapture cap(path.toStdString());
    cap.set(CV_CAP_PROP_POS_FRAMES, frame_num);
    cap >> frame;

    switch (frame.type()) {
        case CV_8UC1:
            cvtColor(frame, frame, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(frame, frame, CV_BGR2RGB);
            break;
    }

    Analysis* ana = m_analysis->load_analysis();
    // Iterate over all detections on the frame
    std::vector<cv::Rect> detections = ana->get_detections_on_frame(frame_num);
    for (cv::Rect rect : detections) {
        YoloWidgetItem* y_item = new YoloWidgetItem(this);
        y_item->setText(QString::number(frame_num));
        cv::Mat dst;
        cv::resize(frame(rect), dst, cv::Size(rect.width,rect.height));

        ImageGenerator im_gen(dst, m_proj->get_dir());
        QString thumbnail_path = im_gen.create_thumbnail(Utility::name_from_path(path), 250);
        const QIcon icon(thumbnail_path);
        y_item->setIcon(icon);
        addItem(y_item);
        setIconSize(QSize(250,250));
    }
}

void YoloListWidget::set_project(Project* proj) {
    m_proj = proj;
    m_ana_list = m_proj->get_analyses();
}
