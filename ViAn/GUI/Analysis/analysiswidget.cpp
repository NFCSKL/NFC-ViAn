#include "analysiswidget.h"

#include "Analysis/analysismethod.h"
#include "GUI/TreeItems/analysisitem.h"
#include "GUI/TreeItems/videoitem.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/videoproject.h"
#include "queuewidget.h"

#include <QDebug>
#include <QThreadPool>


AnalysisWidget::AnalysisWidget(QWidget *parent) : QWidget(parent){

}

void AnalysisWidget::set_queue_wgt(QueueWidget *queue_wgt){
    m_queue_wgt = queue_wgt;

    connect(m_queue_wgt, &QueueWidget::abort_analysis, this, &AnalysisWidget::abort_analysis);
}

/**
 * @brief AnalysisWidget::start_analysis
 * @param save_path     : path to the save map
 * @param video_path    : full path of the video
 * @param item          : video to be analysed
 * Puts the analysis in the queue and if the was empty starts the analysis directly
 */
void AnalysisWidget::start_analysis(QTreeWidgetItem* item, AnalysisMethod *method) {
    std::tuple<AnalysisMethod*,QTreeWidgetItem*> analys (method,item);
    // Add the analysis to the queue in the queue widget
    m_queue_wgt->enqueue(method);
    emit show_analysis_queue(true);
    if (!analysis_queue.empty()) {
        analysis_queue.push_back(analys);
        QString name = "Queued #" + QString::number(analysis_queue.size()-1);
        emit name_in_tree(item, name);
    } else {
        m_queue_wgt->next();
        analysis_queue.push_back(analys);
        perform_analysis(analys);
        current_analysis_item = item;
    }
}

/**
 * @brief AnalysisWidget::perform_analysis
 * @param analys
 * Actually starts the analysis
 * Takes in a tuple consisting of <savepath, videopath, video to be analysed>
 */
void AnalysisWidget::perform_analysis(std::tuple<AnalysisMethod*, QTreeWidgetItem*> analys) {
    AnalysisMethod* method = std::get<0>(analys);
    bool* abort_bool = new bool(false);
    method->aborted = abort_bool;
    abort_map.insert(std::make_pair(method,abort_bool));    
    current_method = method;

    connect(method, &AnalysisMethod::analysis_aborted, this, &AnalysisWidget::on_analysis_aborted);   
    connect(method, &AnalysisMethod::send_progress, this, &AnalysisWidget::send_progress);
    connect(method, &AnalysisMethod::finished_analysis, this, &AnalysisWidget::analysis_done);
    QThreadPool::globalInstance()->start(method);
    emit add_analysis_bar();
    emit show_analysis_queue(true);
}

/**
 * @brief AnalysisWidget::analysis_done
 * @param analysis
 * Slot function to be called when an analysis is completed
 * Removes the current analysis from the queue and start the next one if there is one
 */
void AnalysisWidget::analysis_done(AnalysisProxy* analysis) {
    emit show_progress(0);
    switch (analysis->get_type()) {
    case MOTION_DETECTION:
        analysis->m_name = "Motion analysis";
        current_analysis_item->setText(0,"Motion analysis");
        break;
    case YOLO:
        analysis->m_name = "Object detection";
        current_analysis_item->setText(0,"Object detection");
        break;
    default:
        break;
    }
    analysis_queue.pop_front();
    AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(current_analysis_item);
    ana_item->set_analysis(analysis);
    ana_item->saved = false;
    VideoItem* vid = dynamic_cast<VideoItem*>(current_analysis_item->parent());
    vid->get_video_project()->add_analysis(analysis);

    current_analysis_item = nullptr;
    duration = 0;

    m_queue_wgt->next();
    if (!analysis_queue.empty()) {
        current_analysis_item = std::get<1>(analysis_queue.front());
        move_queue();
        perform_analysis(analysis_queue.front());
    }else{
        emit show_analysis_queue(false);
    }
    emit remove_analysis_bar();
}

void AnalysisWidget::abort_analysis() {
    bool* abort = abort_map.at(current_method);
    *abort = true;
}

void AnalysisWidget::abort_all_analysis() {
    abort_all = true;
    abort_analysis();
}

void AnalysisWidget::on_analysis_aborted() {
    analysis_queue.pop_front();
    delete current_analysis_item; // Delete item from tree
    auto it = abort_map.find(current_method);
    if (it != abort_map.end()) abort_map.erase(it);

    m_queue_wgt->next();
    if (!analysis_queue.empty()) {
        current_analysis_item = std::get<1>(analysis_queue.front());
        move_queue();
        perform_analysis(analysis_queue.front());
        if (abort_all) {
            abort_analysis();
        }
        return;
    }
    // Queue Empty
    emit show_analysis_queue(false);
    emit remove_analysis_bar();
}

/**
 * @brief AnalysisWidget::move_queue
 * Update all the analyses in the queue with the correct queue number in the project tree
 */
void AnalysisWidget::move_queue() {
    for (unsigned int i = 0; i < analysis_queue.size(); i++) {
        QString name = "Queued #" + QString::number(i);
        emit name_in_tree(std::get<1>(analysis_queue.at(i)), name);
    }
}

/**
 * @brief AnalysisWidget::send_progress
 * @param progress
 * Slot for sending the analysis progress
 */
void AnalysisWidget::send_progress(int progress) {
    int time = (std::clock()-start)/CLOCKS_PER_SEC;
    if (time > duration) {
        duration = time;
        if (dots.length() == 3) {
            dots.clear();
        } else {
            dots += ".";
        }
    }
    m_queue_wgt->update_progress(progress);
    QString name = "Loading " + QString::number(progress) + "%" + dots;
    emit name_in_tree(current_analysis_item, name);
    emit show_progress(progress);
}
