#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

    void on_playButton_clicked();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
