#ifndef YOLOWIDGET_H
#define YOLOWIDGET_H

#include <QListWidget>

class Analysis;
class AnalysisProxy;
class Project;
class YoloWidget : public QListWidget
{
    Q_OBJECT
    Analysis* m_analysis = nullptr;
    Project* m_proj = nullptr;
public:
    YoloWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);

public slots:
    void set_project(Project* proj);
};

#endif // YOLOWIDGET_H
