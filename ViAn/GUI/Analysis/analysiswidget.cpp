#include "analysiswidget.h"
#include "GUI/TreeItems/analysisitem.h"
#include "GUI/videowidget.h"
#include <QDebug>
#include <QTreeWidgetItem>
#include <tuple>
AnalysisWidget::AnalysisWidget(QWidget *parent) {
    queue_wgt = new QueueWidget();
    connect(queue_wgt, SIGNAL(abort_analysis()), this, SLOT(abort_analysis()));
}

/**
 * @brief AnalysisWidget::start_analysis
 * @param save_path     : path to the save map
 * @param video_path    : full path of the video
 * @param item          : video to be analysed
 * Puts the analysis in the queue and if the was empty starts the analysis directly
 */
void AnalysisWidget::start_analysis(QTreeWidgetItem* item, AnalysisMethod *method) {
    tuple<AnalysisMethod*,QTreeWidgetItem*> analys (method,item);
    queue_wgt->enqueue(method);
    if (!analysis_queue.empty()) {
        analysis_queue.push_back(analys);        
        std::string name = "Queued #"+to_string(analysis_queue.size()-1);
        emit name_in_tree(item, QString::fromStdString(name));
    } else {
        queue_wgt->next();
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
void AnalysisWidget::perform_analysis(tuple<AnalysisMethod*, QTreeWidgetItem*> analys) {
    AnalysisMethod* method = get<0>(analys);
    bool* abort_bool = new bool(false);
    method->aborted = abort_bool;
    abort_map.insert(std::make_pair(method,abort_bool));    
    current_method = method;

    connect(method, &AnalysisMethod::analysis_aborted, this, &AnalysisWidget::on_analysis_aborted);
    connect(method, &AnalysisMethod::send_progress, this,&AnalysisWidget::send_progress);       
    connect(method, SIGNAL(send_progress(int)),this, SLOT(send_progress(int)));
    connect(method, &AnalysisMethod::finished_analysis, this, &AnalysisWidget::analysis_done);
    QThreadPool::globalInstance()->start(method);
    emit add_analysis_bar();
    queue_wgt->show();
}

/**
 * @brief AnalysisWidget::analysis_done
 * @param analysis
 * Slot function to be called when an analysis is completed
 * Removes the current analysis from the queue and start the next one if there is one
 */
void AnalysisWidget::analysis_done(AnalysisProxy analysis) { 
    emit name_in_tree(current_analysis_item, "Analysis");
    emit show_progress(0);
    analysis_queue.pop_front();
    AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(current_analysis_item);
    AnalysisProxy* am = new AnalysisProxy(analysis);
    ana_item->set_analysis(am);
    VideoItem* vid = dynamic_cast<VideoItem*>(current_analysis_item->parent());
    vid->get_video_project()->add_analysis(am);
    current_analysis_item = nullptr;
    duration = 0;


    queue_wgt->next();
    if (!analysis_queue.empty()) {
        current_analysis_item = get<1>(analysis_queue.front());
        move_queue();
        perform_analysis(analysis_queue.front());
    }else{
        queue_wgt->hide();
    }
    emit remove_analysis_bar();
}

void AnalysisWidget::abort_analysis()
{
    bool* abort = abort_map.at(current_method);
    *abort = true;
}

void AnalysisWidget::on_analysis_aborted()
{    
    analysis_queue.pop_front();
    delete current_analysis_item; // Delete item from tree
    auto it = abort_map.find(current_method);   
    abort_map.erase(it);

    queue_wgt->next();
    if (!analysis_queue.empty()) {        
        current_analysis_item = get<1>(analysis_queue.front());
        move_queue();
        perform_analysis(analysis_queue.front());
        return;
    }
    // Queue Empty
    queue_wgt->hide();
    emit remove_analysis_bar();
}

/**
 * @brief AnalysisWidget::move_queue
 * Update all the analyses in the queue with the correct queue number in the project tree
 */
void AnalysisWidget::move_queue() {
    for (unsigned int i = 0; i < analysis_queue.size(); i++) {
        std::string name = "Queued #"+to_string(i);
        emit name_in_tree(get<1>(analysis_queue.at(i)), QString::fromStdString(name));
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
    queue_wgt->update_progress(progress);
    std::string name = "Loading " + to_string(progress) + "%" + dots;
    emit name_in_tree(current_analysis_item, QString::fromStdString(name));
    emit show_progress(progress);
}
