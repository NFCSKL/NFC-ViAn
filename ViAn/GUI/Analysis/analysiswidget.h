#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include "GUI/videowidget.h"
#include <QWidget>
#include <QTreeWidgetItem>
#include <ctime>
#include "GUI/TreeItems/videoitem.h"
#include "Project/videoproject.h"
#include "queuewidget.h"
#include <QThreadPool>
#include <map>
class AnalysisWidget : public QWidget
{
    Q_OBJECT

    std::map<AnalysisMethod*, bool*> abort_map;
public:
    explicit AnalysisWidget(QWidget *parent = nullptr);
    QueueWidget* queue_wgt;
    std::string dots = "";
    std::clock_t start;
    int duration = 0;
    std::deque<tuple<AnalysisMethod*,QTreeWidgetItem*>> analysis_queue;
    QTreeWidgetItem* current_analysis_item;
    AnalysisMethod* current_method;
private:
    void perform_analysis(tuple<AnalysisMethod *, QTreeWidgetItem *> analys);
    void move_queue();
public slots:
    void start_analysis(QTreeWidgetItem* item, AnalysisMethod *method);
    void send_progress(int);
    void analysis_done(AnalysisProxy);
    void abort_analysis();
    void on_analysis_aborted();
signals:
    void add_analysis_bar(void);
    void remove_analysis_bar(void);
    void show_progress(int);
    void name_in_tree(QTreeWidgetItem*, QString);
    void add_slider_interval(int, int);
};

#endif // ANALYSISWIDGET_H
