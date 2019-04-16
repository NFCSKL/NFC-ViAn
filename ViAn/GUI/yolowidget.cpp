#include "yolowidget.h"

#include "Project/Analysis/analysis.h"
#include "Project/Analysis/analysisproxy.h"

YoloWidget::YoloWidget(QWidget *parent) : QListWidget(parent) {
    setMinimumSize(1000, 1000);
    setViewMode(QListWidget::IconMode);
    setResizeMode(QListWidget::Adjust);
}

void YoloWidget::set_analysis(AnalysisProxy* analysis) {
    m_analysis = analysis->load_analysis();

    for (auto interval : m_analysis->get_intervals()) {
        addItem(QString::number(interval->get_start()));
    }
}
