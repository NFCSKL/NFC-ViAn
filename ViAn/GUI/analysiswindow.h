#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include "Library/customdialog.h"
#include "mainwindow.h"
#include "Filehandler/filehandler.h"
#include "projectmanager.h"
class MainWindow;
namespace Ui {
class AnalysisWindow;
}

class AnalysisWindow : public QMainWindow
{
    Q_OBJECT
    static constexpr int STATUS_BAR_TIMER = 750; // Time status shows in milliseconds
public:
    explicit AnalysisWindow(MainWindow *mainwindow, ProjectManager *project_manager, QWidget *parent = 0);
    void set_current_video(MyQTreeWidgetItem *current_video);
    void remove_analysis_from_list(ID id);
    void set_progress_bar(int progress);
    ~AnalysisWindow();

private slots:
    void on_add_button_clicked();
private:
    Ui::AnalysisWindow *ui;
    MainWindow *mainwindow;
    ProjectManager *project_manager;
    void set_status_bar(std::string status, int timer = STATUS_BAR_TIMER);
    MyQTreeWidgetItem *current_video;
};

#endif // ANALYSISWINDOW_H
