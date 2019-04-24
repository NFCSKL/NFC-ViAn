#ifndef YOLOWIDGET_H
#define YOLOWIDGET_H

#include <QWidget>

class AnalysisProxy;
class Project;
class QComboBox;
class QSlider;
class YoloListWidget;
class YoloWidget : public QWidget
{
    Q_OBJECT
    YoloListWidget* m_list = nullptr;
    QSlider* frames_slider;
    QComboBox* frames_combo;
public:
    YoloWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);

public slots:
    void set_project(Project* proj);

private slots:
    void update_frame_list(std::vector<int> frame_list);

signals:
    void set_frame(int);
};

#endif // YOLOWIDGET_H
