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
    }
}

/**
 * @brief MakeProject::on_ok_button_clicked
 * Creates a project and adds it to the project_tree.
 * Then deletes itself.
 */
void MakeProject::on_ok_button_clicked() {
    Project *proj = file_handler->create_project(name, project_path.toStdString());
    mainwindow->add_project_to_tree(proj);
    mainwindow->set_status_bar("Project " + name.toStdString() + " created.");
    delete this;
}

/**
 * @brief MakeProject::on_cancel_button_clicked
 * Just deletes itself.
 */
void MakeProject::on_cancel_button_clicked() {
    delete this;
}

/**
 * @brief MakeProject::on_project_path_input_textChanged
 * @param arg1 the text of the input box
 * Sets project_path to what is inside the input box.
 */
void MakeProject::on_project_path_input_textChanged(const QString &arg1) {
    this->project_path = arg1;
}

/**
 * @brief MakeProject::on_video_path_input_textChanged
 * @param arg1 the text of the input box
 * Sets video_path to what is inside the input box.
 */
void MakeProject::on_video_path_input_textChanged(const QString &arg1) {
    this->video_path = arg1;
}

/**
 * @brief MakeProject::on_name_input_textChanged
 * @param arg1 the text of the input box
 * Sets name to what is inside the input box.
 */
void MakeProject::on_name_input_textChanged(const QString &arg1) {
    this->name = arg1;
}
