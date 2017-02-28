#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <string>
#include <QCloseEvent>
#include "icononbuttonhandler.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void setStatusBar(string status, int timer);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_playPauseButton_clicked();

    void on_stopButton_clicked();

    void on_videoSlider_valueChanged(int newPos);

    void on_actionExit_triggered();

    void setShortcuts();

    void closeEvent (QCloseEvent *event);

private:

    Ui::MainWindow *ui;
    IconOnButtonHandler *iconOnButtonHandler;
    bool playing;
};

#endif // MAINWINDOW_H
