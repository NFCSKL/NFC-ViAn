#include "yololistwidget.h"

#include "GUI/YoloWidget/itemcreator.h"
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
#include <QProgressDialog>
#include <QThread>

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
    QString video_path = m_proxy->get_video_path();

    ItemCreator* item_creator = new ItemCreator(m_analysis, &m_detection_list,
                                                m_frame_list, video_path, m_proj->get_dir());


    QThread* create_thread = new QThread;
    item_creator->moveToThread(create_thread);
    connect(create_thread, &QThread::started, item_creator, &ItemCreator::create_detection_items);
    connect(item_creator, &ItemCreator::finished, create_thread, &QThread::quit);
    connect(item_creator, &ItemCreator::finished, item_creator, &ItemCreator::deleteLater);
    connect(create_thread, &QThread::finished, create_thread, &QThread::deleteLater);
    connect(item_creator, &ItemCreator::send_last_frame, this, &YoloListWidget::set_last_frame);
    connect(item_creator, &ItemCreator::detection_added, this, &YoloListWidget::filter_detections);
    if (m_frame_list.size() != 0) {
        QProgressDialog* progress = new QProgressDialog(
                    "Loading detections...", "Close", 0, m_frame_list.size(), this, Qt::WindowMinimizeButtonHint);
        connect(item_creator, &ItemCreator::update_progress, progress, &QProgressDialog::setValue);
        progress->show();
    }

    create_thread->start();

    filter_detections();
    update_slider();
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
        } else if (y_item->class_name == m_class_name) {
            class_bool = true;
        }
        // Check frame
        if (m_frame == -1) {
            frame_bool = true;
        } else if (exact_frame && y_item->frame_nr == m_frame) {
            frame_bool = true;
        } else if (!exact_frame && y_item->frame_nr >= m_frame) {
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
    emit number_items(count());
}

void YoloListWidget::update_frame_filter(QString text) {
    bool ok;
    int num = text.toInt(&ok);
    if (!ok) num = -1;
    m_frame = num;
    filter_detections();
    emit set_slider(num);
}

void YoloListWidget::update_frame_filter_int(int frame) {
    m_frame = frame;
    filter_detections();
    emit set_slider(frame);
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

void YoloListWidget::set_last_frame(int frame) {
    last_frame = frame;
    update_slider();
}

void YoloListWidget::set_project(Project* proj) {
    m_proj = proj;
}

void YoloListWidget::set_exact_frame_bool(bool b) {
    exact_frame = b;
    filter_detections();
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
