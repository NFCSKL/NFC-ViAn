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
        mainwindow->add_analysis_to_tree(ui->name_input->text());
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

/**
 * @brief NewAnalysis::get_fps
 * Requests the user to enter FPS.
 * @return Returns the FPS, or -1 if cancelled.
 */
float NewAnalysis::get_fps() {
    float fps = FPS_DEFAULT;

    // Create the texts shown in the dialog
    QString fps_text = QString("FPS [");
    fps_text.append(QString::number(FPS_MIN));
    fps_text.append(" - ");
    fps_text.append(QString::number(FPS_MAX));
    fps_text.append("]: ");

    // Create the dialog
    CustomDialog dialog("FPS", this);
    dialog.addLabel("Enter value:");
    dialog.addDblSpinBoxF(fps_text, FPS_MIN, FPS_MAX, &fps, FPS_DECIMALS, FPS_STEP,
                          "Choose FPS value with the input box.");

    // Show the dialog (execution will stop here until the dialog is finished)
    dialog.exec();

    if (dialog.wasCancelled()) {
        return -1;
    }
    return fps;
}
