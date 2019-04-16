#ifndef YOLOWIDGET_H
#define YOLOWIDGET_H

#include <QListWidget>

class Analysis;
class AnalysisProxy;
class YoloWidget : public QListWidget
{
    Analysis* m_analysis = nullptr;
public:
    YoloWidget(QWidget* parent = nullptr);

    void set_analysis(AnalysisProxy* analysis);
};

#endif // YOLOWIDGET_H
