#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <QSlider>
#include "Video/video_player.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"
#include "icononbuttonhandler.h"

using namespace std;
using namespace cv;


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

    void on_nextFrameButton_clicked();
    void on_previousFrameButton_clicked();

    void update_video(QImage frame);
    void set_video_slider_pos(int pos);

    void resizeEvent(QResizeEvent* event);
private:

    Ui::MainWindow *ui;
    video_player* mvideo_player;
    IconOnButtonHandler *iconOnButtonHandler;

    QSlider *video_slider;
    void on_videoSlider_valueChanged(int newPos);


};

#endif // MAINWINDOW_H
