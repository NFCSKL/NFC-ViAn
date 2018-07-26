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

/**
 * @brief The AnalysisWidget class
 * This class manages all analysis widgets
 */
class AnalysisWidget : public QWidget
{
    Q_OBJECT
    /**
     * @brief abort_map
     * Map containing abort bool for analysismethods,
     * settings this bool to true will abort the analysis
     * which it is mapped from
     */
    std::map<AnalysisMethod*, bool*> abort_map;

    std::string dots = "";
    std::clock_t start;
    int duration = 0;
    int duplicate_save_file_inc = 1;
    bool abort_all = false;
public:
    explicit AnalysisWidget(QWidget *parent = nullptr);

    // Internal queue for analyses
    std::deque<std::tuple<AnalysisMethod*,QTreeWidgetItem*>> analysis_queue;

    // Widget representing the current analysis queue
    QueueWidget* m_queue_wgt;

    // Item in project tree corresponding to
    // the currently executing method
    QTreeWidgetItem* current_analysis_item;

    // Currently executing analysismethod
    AnalysisMethod* current_method;
private:
    // Execute analysis
    void perform_analysis(std::tuple<AnalysisMethod *, QTreeWidgetItem *> analys);

    // Moves the queue forward
    void move_queue();

public slots:
    void set_queue_wgt(QueueWidget* queue_wgt);

    // Incoming request to start analysis
    void start_analysis(QTreeWidgetItem* item, AnalysisMethod *method);

    // Recieve progress from current analysis
    void send_progress(int);

    // Analysis has finished executing
    void analysis_done(AnalysisProxy *);

    // Answer request for aborting analysis
    void abort_analysis();

    // Answer request for aborting all analysis
    void abort_all_analysis();

    // Analysis finished aborting
    void on_analysis_aborted();
signals:
    // Show analysis progress bar
    void add_analysis_bar(void);

    // Hide analysis progress bar
    void remove_analysis_bar(void);

    // Send progress from analysis
    void show_progress(int);

    // Set analysis name in project tree
    void name_in_tree(QTreeWidgetItem*, QString);

    // Show or hide the analysis queue window
    void show_analysis_queue(bool);
};

#endif // ANALYSISWIDGET_H
