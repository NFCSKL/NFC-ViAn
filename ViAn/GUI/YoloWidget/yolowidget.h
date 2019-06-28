#ifndef YOLOWIDGET_H
#define YOLOWIDGET_H

#include <QWidget>

class AnalysisProxy;
class AnalysisSlider;
class Project;
class QCheckBox;
class QComboBox;
class QLabel;
class QPushButton;
class QSlider;
class VideoProject;
class YoloListWidget;
class YoloWidget : public QWidget
{
    Q_OBJECT
    YoloListWidget* m_list = nullptr;
    QComboBox* frames_combo;
    QPushButton* next_btn;
    QPushButton* prev_btn;
    QCheckBox* exact_frame_box;
    QSlider* confidence_slider;
    QLabel* conf_value;
    QComboBox* classes_combo;
    QSlider* icon_size_slider;
    QLabel* icon_size_label;
    QLabel* detection_number_label;
    AnalysisSlider* frames_slider;
    QLabel* curr_time;
    QLabel* end_time;

    std::vector<AnalysisProxy*> m_ana_list;
public:
    YoloWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);

public slots:
    void set_project(Project* proj);
    void set_slider_value(int value);
    void set_detection_number(int number);

private slots:
    void set_classes();
    void update_conf_value(int value);
    void update_frame_list(std::vector<int> frame_list);
    void set_slider_max(int);
    void prev_btn_clicked();
    void next_btn_clicked();
    void set_icon_size(int value);

signals:
    void set_frame(VideoProject*, int);
};

#endif // YOLOWIDGET_H
