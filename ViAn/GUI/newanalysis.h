#ifndef NEWANALYSIS_H
#define NEWANALYSIS_H

#include <QMainWindow>
#include "mainwindow.h"
#include "Filehandler/filehandler.h"

class MainWindow;
namespace Ui {
class NewAnalysis;
}

class NewAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewAnalysis(MainWindow *mainwindow, FileHandler *file_handler, QWidget *parent = 0);
    ~NewAnalysis();

private slots:
    void on_add_button_clicked();

private:
    Ui::NewAnalysis *ui;
    MainWindow *mainwindow;
    FileHandler *file_handler;
    void set_status_bar(std::string status, int timer = 750);
};

#endif // NEWANALYSIS_H
