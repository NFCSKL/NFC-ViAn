#ifndef MAKEPROJECT_H
#define MAKEPROJECT_H

#include <QMainWindow>
#include <QString>
#include "Filehandler/filehandler.h"
#include "mainwindow.h"

class MainWindow;
namespace Ui {
class MakeProject;
}

class MakeProject : public QMainWindow
{
    Q_OBJECT

public:
    explicit MakeProject(MainWindow *mainwindow, FileHandler *file_handler, std::string worksace, QWidget *parent = 0);
    ~MakeProject();

private slots:
    void on_project_path_search_button_clicked();

    void on_video_path_search_button_clicked();

    void on_ok_button_clicked();

    void on_cancel_button_clicked();

    void on_name_input_textChanged(const QString &name);

private:
    QString project_path;
    QString video_path;
    QString name;
    FileHandler *file_handler;
    MainWindow *mainwindow;
    Ui::MakeProject *ui;
    void set_status_bar(std::string status, int timer = 750);
};

#endif // MAKEPROJECT_H
