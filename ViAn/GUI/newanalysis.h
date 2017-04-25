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
    void on_ok_button_clicked();

private:
    Ui::NewAnalysis *ui;
    MainWindow *mainwindow;
    FileHandler *file_handler;
};

#endif // NEWANALYSIS_H
