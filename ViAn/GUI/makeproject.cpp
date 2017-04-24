#include "makeproject.h"
#include "ui_makeproject.h"
#include <QFileDialog>
#include <QShortcut>

/**
 * @brief MakeProject::MakeProject
 * @param mainwindow
 * @param file_handler
 * @param workspace, the standard
 * @param parent
 */
MakeProject::MakeProject(MainWindow *mainwindow, FileHandler *file_handler, std::string workspace, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MakeProject) {
    ui->setupUi(this);
    this->project_path = QString::fromStdString(workspace);
    ui->project_path_input->setText(this->project_path); //sets the text in the input box
    this->video_path = QString::fromStdString(workspace);
    ui->video_path_input->setText(this->video_path); //sets the text in the input box
    this->file_handler = file_handler;
    this->mainwindow = mainwindow;
    QShortcut *ok_shortcut = new QShortcut(QKeySequence(tr("Return")), this);
    QObject::connect(ok_shortcut, SIGNAL(activated()), this, SLOT(on_ok_button_clicked()));
    QShortcut *cancel_shortcut = new QShortcut(QKeySequence(tr("Esc")), this);
    QObject::connect(cancel_shortcut, SIGNAL(activated()), this, SLOT(on_cancel_button_clicked()));
}

/**
 * @brief MakeProject::~MakeProject
 */
MakeProject::~MakeProject() {
    delete ui;
}

/**
 * @brief MakeProject::on_project_path_search_button_clicked
 * Gets a directort and sets the text in the input box.
 */
void MakeProject::on_project_path_search_button_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose path to project"),this->project_path);
    if(!dir.isEmpty()) {
        ui->project_path_input->setText(dir);
        this->project_path = dir;
    }
}

/**
 * @brief MakeProject::on_video_path_search_button_clicked
 * Gets a directort and sets the text in the input box.
 */
void MakeProject::on_video_path_search_button_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose path to videos"),this->video_path);
    if(!dir.isEmpty()) {
        ui->video_path_input->setText(dir);
        this->video_path = dir;
    }
}

/**
 * @brief MakeProject::on_ok_button_clicked
 * Creates a project and adds it to the project_tree.
 * Then deletes itself.
 */
void MakeProject::on_ok_button_clicked() {
    if(this->name.isEmpty())
        set_status_bar("Needs a name");
    else if(this->project_path.isEmpty())
        set_status_bar("Needs a path for the project");
    else if(this->video_path.isEmpty())
        set_status_bar("Needs a path for the videos");
   else {
        Project *proj = file_handler->create_project(name, project_path.toStdString());
        mainwindow->add_project_to_tree(proj);
        set_status_bar("Project " + name.toStdString() + " created.");
        delete this;
    }
}

/**
 * @brief MakeProject::on_cancel_button_clicked
 * Just deletes itself.
 */
void MakeProject::on_cancel_button_clicked() {
    delete this;
}

/**
 * @brief MakeProject::on_name_input_textChanged
 * @param arg1 the text of the input box
 * Sets name to what is inside the input box.
 */
void MakeProject::on_name_input_textChanged(const QString &name) {
    this->name = name;
}

/**
 * @brief MakeProject::set_status_bar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void MakeProject::set_status_bar(std::string status, int timer){
    ui->statusbar->showMessage(QString::fromStdString(status), timer);
    mainwindow->set_status_bar(status, timer);
}
