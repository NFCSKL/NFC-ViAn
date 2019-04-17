#include "yolowidget.h"

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

YoloWidget::YoloWidget(QWidget *parent) : QListWidget(parent) {
    setMinimumSize(1000, 1000);
    setViewMode(QListWidget::IconMode);
    setResizeMode(QListWidget::Adjust);
}

void YoloWidget::set_analysis(AnalysisProxy* analysis) {
    m_analysis = analysis->load_analysis();
    VideoProject* vid_proj = m_proj->get_video_project(m_analysis->get_vid_proj_id());
    QString path = vid_proj->get_video()->file_path;
    cv::Mat frame;
    cv::VideoCapture cap(path.toStdString());

    for (auto interval : m_analysis->get_intervals()) {
        int frame_nr = interval->get_start();
        cap.set(CV_CAP_PROP_POS_FRAMES, frame_nr);
        cap >> frame;

        std::vector<cv::Rect> detections = m_analysis->get_detections_on_frame(interval->get_start());
        for (cv::Rect rect : detections) {
            YoloWidgetItem* y_item = new YoloWidgetItem(this);
            y_item->setText(QString::number(frame_nr));

            //rect.tl()

            ImageGenerator im_gen(frame, m_proj->get_dir());
            QString thumbnail_path = im_gen.create_thumbnail(Utility::name_from_path(path));
            const QIcon icon(thumbnail_path);
            y_item->setIcon(icon);
            addItem(y_item);
        }
    }
}

void YoloWidget::set_project(Project* proj) {
    m_proj = proj;
}
