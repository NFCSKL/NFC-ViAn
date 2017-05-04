#include "analysiswindow.h"
#include "ui_analysiswindow.h"
#include <iostream>


AnalysisWindow::AnalysisWindow(MainWindow *mainwindow, FileHandler *file_handler, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow) {
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    this->file_handler = file_handler;
    for(auto it = ANALYSIS_NAMES.begin(); it!=ANALYSIS_NAMES.end(); it++) {
        ui->analysis_choise_list->addItem(QString::fromStdString(*it));
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
    if(!ui->name_input->text().isEmpty()) {
        mainwindow->add_analysis_to_tree(ANALYSIS_TYPE::MOTION_DETECTION, ui->name_input->text(), current_video);
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

void AnalysisWindow::set_current_video(MyQTreeWidgetItem *current_video) {
    this->current_video = current_video;
    std::string text = "Add analysis to video: " + current_video->get_name();
    ui->current_video_lable->setText(QString::fromStdString(text));
}

void AnalysisWindow::remove_analysis_from_list(ID id) {
    delete ui->analysis_list->takeItem(id);
}
