#include "makeproject.h"
#include "ui_makeproject.h"
#include <QFileDialog>
#include <QShortcut>

MakeProject::MakeProject(MainWindow *mainwindow, FileHandler *file_handler, std::string workspace, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MakeProject)
{
    ui->setupUi(this);
    this->project_path = QString::fromStdString(workspace);
    ui->project_path_input->setText(this->project_path);
    this->video_path = QString::fromStdString(workspace);
    ui->video_path_input->setText(this->video_path);
    this->file_handler = file_handler;
    this->mainwindow = mainwindow;
    QShortcut *ok_shortcut = new QShortcut(QKeySequence(tr("Return")), this);
    QObject::connect(ok_shortcut, SIGNAL(activated()), this, SLOT(on_ok_button_clicked()));
    QShortcut *cancel_shortcut = new QShortcut(QKeySequence(tr("Esc")), this);
    QObject::connect(cancel_shortcut, SIGNAL(activated()), this, SLOT(on_cancel_button_clicked()));
}

MakeProject::~MakeProject()
{
    delete ui;
}

void MakeProject::on_project_path_search_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose path to project"),this->project_path);
    if(!dir.isEmpty()) {
        this->project_path = dir;
        ui->project_path_input->setText(this->project_path);
    }
}

void MakeProject::on_video_path_search_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose path to videos"),this->video_path);
    if(!dir.isEmpty()) {
        this->video_path = dir;
        ui->video_path_input->setText(this->video_path);
    }
}

void MakeProject::on_ok_button_clicked()
{
    Project *proj = file_handler->create_project(name, project_path.toStdString());
    mainwindow->add_project_to_tree(proj);
    mainwindow->set_status_bar("Project " + name.toStdString() + " created.");
    delete this;
}

void MakeProject::on_cancel_button_clicked()
{
    delete this;
}

void MakeProject::on_video_path_input_editingFinished()
{
    this->video_path = ui->video_path_input->text();
}

void MakeProject::on_project_path_input_editingFinished()
{
    this->project_path = ui->project_path_input->text();
}

void MakeProject::on_name_input_editingFinished()
{
    this->name = ui->name_input->text();
}
