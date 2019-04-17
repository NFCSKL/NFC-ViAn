#ifndef YOLOWIDGET_H
#define YOLOWIDGET_H

#include <QListWidget>

class Analysis;
class AnalysisProxy;
class Project;
class YoloWidget : public QListWidget
{
    Analysis* m_analysis = nullptr;
    Project* m_proj = nullptr;
public:
    YoloWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);
    void set_project(Project* proj);
};

#endif // YOLOWIDGET_H
