#include "yolowidget.h"

#include "Project/Analysis/analysisproxy.h"
#include "Project/videoproject.h"
#include "yololistwidget.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QSlider>

YoloWidget::YoloWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    frames_slider = new QSlider(Qt::Horizontal);
    frames_slider->setDisabled(true);
    frames_combo = new QComboBox();
    frames_combo->setDisabled(true);
    frames_combo->addItem("---");

    m_list = new YoloListWidget();

    layout->addWidget(frames_combo);
    layout->addWidget(m_list);
    layout->addWidget(frames_slider);

    layout->setMargin(5);
    layout->setSpacing(5);
    setLayout(layout);

    connect(m_list, &YoloListWidget::update_frames, this, &YoloWidget::update_frame_list);
    connect(frames_combo, &QComboBox::currentTextChanged, m_list, &YoloListWidget::choose_ana_frame);
    connect(m_list, &YoloListWidget::set_frame, this, &YoloWidget::set_frame);
}

void YoloWidget::set_analysis(AnalysisProxy* analysis) {
    switch (analysis->get_type()) {
    case MOTION_DETECTION:
        setWindowTitle("Motion detection - " + analysis->get_name());
        break;
    case OBJECT_DETECTION:
        setWindowTitle("Object detection - " + analysis->get_name());
        break;
    default:
        break;
    }
    m_list->set_analysis(analysis);
}

void YoloWidget::set_project(Project* proj) {
    m_list->set_project(proj);
}

void YoloWidget::update_frame_list(std::vector<int> frame_list) {
    frames_slider->setMaximum(static_cast<int>(frame_list.size()));
    frames_slider->setEnabled(true);

    frames_combo->setEnabled(true);
    frames_combo->clear();
    frames_combo->addItem("---");
    for (int frame : frame_list) {
        frames_combo->addItem(QString::number(frame), frame);
    }
}
