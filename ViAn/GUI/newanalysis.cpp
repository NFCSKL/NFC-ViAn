#include "newanalysis.h"
#include "ui_newanalysis.h"


NewAnalysis::NewAnalysis(MainWindow *mainwindow, FileHandler *file_handler, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewAnalysis)
{
    ui->setupUi(this);
    this->mainwindow = mainwindow;
    this->file_handler = file_handler;
    ui->analysis_choise_list->addItem("Movement");
    ui->analysis_choise_list->addItem("Face detection");
}

/**
 * @brief NewAnalysis::~NewAnalysis
 */
NewAnalysis::~NewAnalysis() {
    delete ui;
}

/**
 * @brief NewAnalysis::on_ok_button_clicked
 */
void NewAnalysis::on_ok_button_clicked() {
    mainwindow->add_analysis_to_tree(ui->analysis_choise_list->currentText());
    delete this;
}
