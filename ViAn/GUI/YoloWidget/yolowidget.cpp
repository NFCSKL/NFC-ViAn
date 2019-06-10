#include "yolowidget.h"

#include "Analysis/analysissettings.h"
#include "GUI/Analysis/analysisslider.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "yololistwidget.h"
#include "utility.h"

#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QDebug>

YoloWidget::YoloWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout();

    // Drop down for choosing frame
    frames_combo = new QComboBox();
    frames_combo->setDisabled(true);
    frames_combo->addItem("---");
    // Prev and next button for choosing frame
    prev_btn = new QPushButton(QIcon(":/Icons/prev_frame.png"), "", this);
    next_btn = new QPushButton(QIcon(":/Icons/next_frame.png"), "", this);
    prev_btn->setFixedSize(22, 22);
    next_btn->setFixedSize(22, 22);

    // Drop down for classes
    classes_combo = new QComboBox();
    set_classes();

    // Slider for confidence
    confidence_slider = new QSlider(Qt::Horizontal);
    confidence_slider->setMinimum(0);
    confidence_slider->setMaximum(100);
    confidence_slider->setPageStep(1);
    confidence_slider->setValue(50);
    QLabel* conf_label = new QLabel("Confidence: ");
    conf_value = new QLabel();
    conf_value->setText(QString::number(confidence_slider->value()) + "%");

    icon_size_slider = new QSlider(Qt::Horizontal);
    icon_size_slider->setMaximum(500);
    icon_size_slider->setMinimum(100);
    icon_size_slider->setValue(250);

    frames_slider = new AnalysisSlider(Qt::Horizontal, this);
    curr_time = new QLabel("Start");
    end_time = new QLabel("End");

    // Add to layouts
    QHBoxLayout* top_layout = new QHBoxLayout();

    QHBoxLayout* frames_layout = new QHBoxLayout();
    frames_layout->addWidget(frames_combo);
    frames_layout->addWidget(prev_btn);
    frames_layout->addWidget(next_btn);
    top_layout->addLayout(frames_layout);
    top_layout->addWidget(classes_combo);

    QHBoxLayout* icon_size_layout = new QHBoxLayout();
    icon_size_label = new QLabel("Icon size: ");
    icon_size_layout->addWidget(icon_size_label);
    icon_size_layout->addWidget(icon_size_slider);

    QHBoxLayout* conf_layout = new QHBoxLayout();
    conf_layout->addWidget(conf_label);
    conf_layout->addWidget(conf_value);
    conf_layout->addWidget(confidence_slider);

    m_list = new YoloListWidget();

    QHBoxLayout* ana_slider_layout = new QHBoxLayout();
    ana_slider_layout->addWidget(curr_time);
    ana_slider_layout->addWidget(frames_slider);
    ana_slider_layout->addWidget(end_time);

    layout->addLayout(top_layout);
    layout->addLayout(conf_layout);
    layout->addLayout(icon_size_layout);
    layout->addWidget(m_list);
    layout->addLayout(ana_slider_layout);

    layout->setMargin(5);
    layout->setSpacing(5);
    setLayout(layout);
    set_icon_size(icon_size_slider->value());

    connect(m_list, &YoloListWidget::update_frames, this, &YoloWidget::update_frame_list);
    connect(frames_combo, &QComboBox::currentTextChanged, m_list, &YoloListWidget::update_frame_filter);
    connect(classes_combo, &QComboBox::currentTextChanged, m_list, &YoloListWidget::update_class_filter);
    connect(confidence_slider, &QSlider::valueChanged, this, &YoloWidget::update_conf_value);
    connect(confidence_slider, &QSlider::valueChanged, m_list, &YoloListWidget::update_confidence_filter);
    connect(prev_btn, &QPushButton::clicked, this, &YoloWidget::prev_btn_clicked);
    connect(next_btn, &QPushButton::clicked, this, &YoloWidget::next_btn_clicked);
    connect(m_list, &YoloListWidget::slider_max, this, &YoloWidget::set_slider_max);
    connect(m_list, &YoloListWidget::set_frame, this, &YoloWidget::set_frame);

    connect(m_list, &YoloListWidget::set_slider, this, &YoloWidget::set_slider_value);
    connect(icon_size_slider, &QSlider::valueChanged, this, &YoloWidget::set_icon_size);
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
    frames_slider->set_analysis_proxy(analysis);
    int conf = int(analysis->get_settings()->get_object_setting("Confidence threshold")*100);
    confidence_slider->setMinimum(conf);
}

void YoloWidget::set_project(Project* proj) {
    m_list->set_project(proj);
    m_ana_list = proj->get_analyses();
}

void YoloWidget::update_conf_value(int value) {
    conf_value->setText(QString::number(value) + "%");
}

void YoloWidget::update_frame_list(std::vector<int> frame_list) {
    frames_combo->setEnabled(true);
    frames_combo->clear();
    frames_combo->addItem("---");
    for (int frame : frame_list) {
        frames_combo->addItem(QString::number(frame), frame);
    }
}

void YoloWidget::set_slider_max(int max) {
    frames_slider->setMaximum(max);
    frames_slider->set_show_on_slider(true);
    frames_slider->set_show_pois(true);
    frames_slider->update();
}

void YoloWidget::prev_btn_clicked() {
    if (frames_combo->currentIndex() != 0) {
        frames_combo->setCurrentIndex(frames_combo->currentIndex()-1);
    }
}

void YoloWidget::next_btn_clicked() {
    if (frames_combo->currentIndex() != frames_combo->count()-1) {
        frames_combo->setCurrentIndex(frames_combo->currentIndex()+1);
    }
}

void YoloWidget::set_slider_value(int value) {
    if (value < 0) value = 0;
    frames_slider->setValue(value);
    end_time->setText(QString::number(m_list->last_frame));
    curr_time->setText(QString::number(value));
}

void YoloWidget::set_icon_size(int size) {
    m_list->setIconSize(QSize(size, size));
    icon_size_label->setText("Icon size: " + QString::number(size));
}
