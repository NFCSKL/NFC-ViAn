#ifndef YOLOWIDGET_H
#define YOLOWIDGET_H

#include <QWidget>

class AnalysisProxy;
class AnalysisSlider;
class Project;
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
    AnalysisSlider* frames_slider;
    QComboBox* frames_combo;
    QPushButton* next_btn;
    QPushButton* prev_btn;
    QSlider* confidence_slider;
    QLabel* conf_value;
    QComboBox* classes_combo;

    std::vector<AnalysisProxy*> m_ana_list;
public:
    YoloWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);

public slots:
    void set_project(Project* proj);

private slots:
    void set_classes();
    void update_conf_value(int value);
    void update_frame_list(std::vector<int> frame_list);
    void set_slider_max(int);
    void prev_btn_clicked();
    void next_btn_clicked();

signals:
    void set_frame(VideoProject*, int);
};

#endif // YOLOWIDGET_H
