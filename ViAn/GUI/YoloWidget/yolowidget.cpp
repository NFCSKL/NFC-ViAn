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
    // Drop down for choosing frame
    frames_combo = new QComboBox();
    frames_combo->setDisabled(true);
    frames_combo->addItem("---");
    // Drop down for analysies
    analyses_combo = new QComboBox();
    analyses_combo->setDisabled(true);
    // Drop down for classes
    classes_combo = new QComboBox();
    set_classes();


    QHBoxLayout* top_layout = new QHBoxLayout(this);
    top_layout->addWidget(frames_combo);
    top_layout->addWidget(analyses_combo);
    top_layout->addWidget(classes_combo);

    m_list = new YoloListWidget();

    layout->addLayout(top_layout);
    layout->addWidget(m_list);
    layout->addWidget(frames_slider);

    layout->setMargin(5);
    layout->setSpacing(5);
    setLayout(layout);

    connect(m_list, &YoloListWidget::update_frames, this, &YoloWidget::update_frame_list);
    connect(frames_combo, &QComboBox::currentTextChanged, m_list, &YoloListWidget::update_frame_filter);
    connect(classes_combo, &QComboBox::currentTextChanged, m_list, &YoloListWidget::update_class_filter);
    connect(m_list, &YoloListWidget::set_frame, this, &YoloWidget::set_frame);
}

void YoloWidget::set_classes() {
    classes_combo->setEnabled(true);
    classes_combo->clear();

    classes_combo->addItem("ALL");
    classes_combo->addItem("aeroplane");
    classes_combo->addItem("backpack");
    classes_combo->addItem("bicycle");
    classes_combo->addItem("boat");
    classes_combo->addItem("bus");
    classes_combo->addItem("car");
    classes_combo->addItem("cat");
    classes_combo->addItem("cell phone");
    classes_combo->addItem("dog");
    classes_combo->addItem("handbag");
    classes_combo->addItem("knife");
    classes_combo->addItem("motorbike");
    classes_combo->addItem("person");
    classes_combo->addItem("suitcase");
    classes_combo->addItem("tie");
    classes_combo->addItem("train");
    classes_combo->addItem("truck");
    classes_combo->addItem("umbrella");
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
