#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <string>
#include <QCloseEvent>
#include <QSlider>
#include "Video/video_player.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"
#include "icononbuttonhandler.h"
#include "ui_mainwindow.h"

using namespace std;


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

    void on_actionExit_triggered();

    void setShortcuts();

    void closeEvent (QCloseEvent *event);

    void update_video(QImage frame);

    void set_video_slider_pos(int pos);

    void resizeEvent(QResizeEvent* event);

    void on_previousFrameButton_clicked();

    void on_nextFrameButton_clicked();

    void on_fastBackwardButton_clicked();

    void on_fastForwardButton_clicked();

    void on_bookmarkButton_clicked();

    void on_videoSlider_sliderPressed();

    void on_videoSlider_sliderReleased();

    void on_videoSlider_valueChanged(int newPos);

private:

    Ui::MainWindow *ui;
    video_player* mvideo_player;
    IconOnButtonHandler *iconOnButtonHandler;

    QSlider *video_slider;
    bool slider_blocked = false;
    bool slider_moving = false;
    int prev_slider_pos = 0;

    int posCounter = 0;
    int changeCounter = 0;

    std::chrono::milliseconds slider_timer = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
            );

};

#endif // MAINWINDOW_H
