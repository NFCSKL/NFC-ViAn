#include "analysiswindow.h"
#include "ui_analysiswindow.h"
#include "GUI/qtreeitems.h"
#include <iostream>

/**
 * @brief AnalysisWindow::AnalysisWindow
 * @param mainwindow
 * @param file_handler
 * @param parent
 */
AnalysisWindow::AnalysisWindow(MainWindow *mainwindow, ProjectManager *project_manager, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow) {
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    this->project_manager = project_manager;
    for(auto it = ANALYSIS_NAMES.begin(); it!=ANALYSIS_NAMES.end(); it++) {
        ui->analysis_choise_list->addItem(QString::fromStdString(*it)); //ads all the types of analyses to the rolldown list.
    }
}

/**
 * @brief AnalysisWindow::~AnalysisWindow
 * Destructor
 */
AnalysisWindow::~AnalysisWindow() {
    delete ui;
}

/**
 * @brief AnalysisWindow::on_ok_button_clicked
 * Adds the selected analysis type to the analysis list.
 */
void AnalysisWindow::on_add_button_clicked() {
    ID proj_id = ((MyQTreeWidgetItem*)mainwindow->get_project_from_object(current_video))->id;
    Project* proj = project_manager->get_project(proj_id);

    if(!ui->name_input->text().isEmpty()) {
        ANALYSIS_TYPE type = ANALYSIS_NAMES_TYPE_MAP.at(ui->analysis_choise_list->currentText().toStdString());
        QString save_path = QString::fromStdString(proj->dir) + "/" + ui->name_input->text() + QString::fromStdString(std::to_string(type));
        mainwindow->add_analysis_to_queue(save_path,type, ui->name_input->text(), current_video, ui->use_analysis_area->isChecked());
        QString text = QString::fromStdString(current_video->get_name() + "/") + ui->name_input->text();
        ui->analysis_list->insertItem(ui->analysis_list->count(), text);
    } else set_status_bar("No name given.");
}

/**
 * @brief AnalysisWindow::set_status_bar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void AnalysisWindow::set_status_bar(std::string status, int timer){
    ui->statusbar->showMessage(QString::fromStdString(status), timer);
    mainwindow->set_status_bar(status, timer);
}

/**
 * @brief AnalysisWindow::set_current_video
 * @param current_video video to do analysis on
 */
void AnalysisWindow::set_current_video(MyQTreeWidgetItem *current_video) {
    this->current_video = current_video;
    std::string text = "Add analysis to video: " + current_video->get_name();
    ui->current_video_lable->setText(QString::fromStdString(text));
}

/**
 * @brief AnalysisWindow::remove_analysis_from_list
 * @param id position in the list
 */
void AnalysisWindow::remove_analysis_from_list(ID id) {
    delete ui->analysis_list->takeItem(id);
}

/**
 * @brief AnalysisWindow::set_progress_bar
 * @param progress percentage
 */
void AnalysisWindow::set_progress_bar(int progress) {
    ui->progress_bar->setValue(progress);
}
