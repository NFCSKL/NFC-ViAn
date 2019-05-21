#ifndef YOLOLISTWIDGET_H
#define YOLOLISTWIDGET_H

#include <QListWidget>

class Analysis;
class AnalysisProxy;
class Project;
class VideoProject;
class YoloListWidget : public QListWidget
{
    Q_OBJECT
    AnalysisProxy* m_analysis = nullptr;
    Project* m_proj = nullptr;
    std::vector<AnalysisProxy*> m_ana_list;
    std::vector<int> m_frame_list;
public:
    YoloListWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);
    void show_frame(int frame_num);
    void update_video_widget(int frame);

public slots:
    void set_project(Project* proj);
    void choose_ana_frame(QString text);

signals:
    void update_frames(std::vector<int>);
    void set_frame(VideoProject*, int);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // YOLOLISTWIDGET_H
