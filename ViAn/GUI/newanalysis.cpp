#include "newanalysis.h"
#include "ui_newanalysis.h"


NewAnalysis::NewAnalysis(MainWindow *mainwindow, FileHandler *file_handler, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewAnalysis) {
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    this->file_handler = file_handler;
    ui->analysis_choise_list->addItem("Movement");
    ui->analysis_choise_list->addItem("Face detection");
    /*for(auto analys = video->analysis.begin(); analys != video->analysis.end(); ++analys) {
        ui->analysi_list->addItem(analys.second());
    }*/
}

/**
 * @brief NewAnalysis::~NewAnalysis
 * Destructor
 */
NewAnalysis::~NewAnalysis() {
    delete ui;
}

/**
 * @brief NewAnalysis::on_ok_button_clicked
 * Adds the selected analysis type to the analysis list.
 */
void NewAnalysis::on_add_button_clicked() {
    if(!ui->name_input->text().isEmpty()) {
        mainwindow->add_analysis_to_tree(ui->name_input->text(), current_video);
        ui->analysis_list->addItem(ui->name_input->text());
    } else set_status_bar("No name given.");
}

/**
 * @brief MakeProject::set_status_bar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void NewAnalysis::set_status_bar(std::string status, int timer){
    ui->statusbar->showMessage(QString::fromStdString(status), timer);
    mainwindow->set_status_bar(status, timer);
}

void NewAnalysis::set_current_video(MyQTreeWidgetItem *current_video) {
    this->current_video = current_video;
    QString text = "Add analysis to video: " + current_video->name;
    ui->current_video_lable->setText(text);
}
