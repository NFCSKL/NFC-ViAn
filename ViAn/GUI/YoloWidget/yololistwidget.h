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
    std::vector<AnalysisProxy*> m_ana_list;
    std::vector<int> m_frame_list;
    std::vector<YoloWidgetItem*> m_detection_list;

    QString m_class_name = "ALL";
    int m_frame = 0;
    double m_confidence = 0.5;
public:
    YoloListWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);
    void show_frame(int frame_num);
    void update_video_widget(int frame);

public slots:
    void create_detection_items();
    void set_project(Project* proj);
    void update_frame_filter(QString text);
    void filter_detections();
    void update_class_filter(QString class_name);

signals:
    void update_frames(std::vector<int>);
    void set_frame(VideoProject*, int);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // YOLOLISTWIDGET_H
