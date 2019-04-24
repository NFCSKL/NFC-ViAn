#ifndef YOLOLISTWIDGET_H
#define YOLOLISTWIDGET_H

#include <QListWidget>

class Analysis;
class AnalysisProxy;
class Project;
class YoloListWidget : public QListWidget
{
    Q_OBJECT
    AnalysisProxy* m_analysis = nullptr;
    Project* m_proj = nullptr;
    std::vector<AnalysisProxy*> m_ana_list;
    std::vector<int> m_frame_list;

    const int ANA_FRAME_STEP = 10;
public:
    YoloListWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);
    void show_frame(int frame_num);

public slots:
    void set_project(Project* proj);
    void choose_ana_frame(QString text);

signals:
    void update_frames(std::vector<int>);
    void set_frame(int);
};

#endif // YOLOLISTWIDGET_H
