#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include "video_player.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"
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

    void update_video(QImage frame);

private:

    Ui::MainWindow *ui;
    video_player* mvideo_player;
};

#endif // MAINWINDOW_H
