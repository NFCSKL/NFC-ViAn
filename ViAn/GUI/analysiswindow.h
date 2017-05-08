#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include "Library/customdialog.h"
#include "mainwindow.h"
#include "Filehandler/filehandler.h"

class MainWindow;
namespace Ui {
class AnalysisWindow;
}

class AnalysisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisWindow(MainWindow *mainwindow, FileHandler *file_handler, QWidget *parent = 0);
    void set_current_video(MyQTreeWidgetItem *current_video);
    void remove_analysis_from_list(ID id);
    void set_progress_bar(int progress);
    ~AnalysisWindow();

private slots:
    void on_add_button_clicked();

private:
    Ui::AnalysisWindow *ui;
    MainWindow *mainwindow;
    FileHandler *file_handler;
    void set_status_bar(std::string status, int timer = 750);
    MyQTreeWidgetItem *current_video;
};

#endif // ANALYSISWINDOW_H
