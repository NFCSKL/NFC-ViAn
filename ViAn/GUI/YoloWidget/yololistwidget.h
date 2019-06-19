#ifndef YOLOLISTWIDGET_H
#define YOLOLISTWIDGET_H

#include <QListWidget>

class Analysis;
class AnalysisProxy;
class Project;
class VideoProject;
class YoloWidgetItem;
class YoloListWidget : public QListWidget
{
    Q_OBJECT
    AnalysisProxy* m_proxy = nullptr;
    Analysis* m_analysis = nullptr;
    Project* m_proj = nullptr;
    std::vector<int> m_frame_list;
    std::vector<YoloWidgetItem*> m_detection_list;

    QString m_class_name = "ALL";
    int m_frame = 0;
    double m_confidence = 0.5;
    bool exact_frame = false;

public:
    YoloListWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);
    void show_frame(int frame_num);
    void update_video_widget(int frame);


    int last_frame = 100;
public slots:
    void set_project(Project* proj);
    void filter_detections();
    void update_frame_filter(QString text);
    void update_frame_filter_int(int frame);
    void update_confidence_filter(int confidence);
    void update_class_filter(QString class_name);

    void update_slider();
    void set_last_frame(int frame);
    void set_exact_frame_bool(bool b);

signals:
    void update_frames(std::vector<int>);
    void set_frame(VideoProject*, int);
    void number_items(int);
    void slider_max(int);
    void set_slider(int frame);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // YOLOLISTWIDGET_H
