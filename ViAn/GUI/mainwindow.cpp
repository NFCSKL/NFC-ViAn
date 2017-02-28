#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;
using namespace cv;

/**
 * @brief MainWindow::MainWindow
 * Constructor
 * @param parent a QWidget variable
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    video_slider = findChild<QSlider*>("videoSlider");

    /**
     * How to get a picture
     */
    /*QImage image;
    image.load("<searchPath>");
    ui->videoFrame->setPixmap(QPixmap::fromImage(image));
    ui->videoFrame->show();*/
    mvideo_player = new video_player();
    QObject::connect(mvideo_player, SIGNAL(processedImage(QImage)),
                                  this, SLOT(update_video(QImage)));
    QObject::connect(mvideo_player, SIGNAL(currentFrame(int)),
                                  this, SLOT(set_video_slider_pos(int)));

    //Used for rescaling the source image for video playback
    mvideo_player->set_frame_height(ui->videoFrame->height());
    mvideo_player->set_frame_width(ui->videoFrame->width());
}

/**
 * @brief MainWindow::~MainWindow
 * Destructor
 */
MainWindow::~MainWindow() {
    delete ui;
}

/**
 * @brief MainWindow::on_playButton_clicked
 * The leftmost button supposed to play the video
 */
void MainWindow::on_playButton_clicked() {

    if (mvideo_player->is_paused()) {
        mvideo_player->start();
    } else {
        mvideo_player->pause();
        mvideo_player->wait();
    }


}

/**
 * @brief MainWindow::on_pauseButton_clicked
 * The middle button supposed to pause the video
 */
void MainWindow::on_pauseButton_clicked() {
    mvideo_player->load_video("seq_01.mp4");
    //mvideo_player->load_video("mf.mkv");
    //mvideo_player->load_video("eng.mov");
    //mvideo_player->load_video("Makefile");

    video_slider->setMaximum(mvideo_player->get_num_frames());
    cout << mvideo_player->get_num_frames() << endl;
}


/**
 * @brief MainWindow::on_stopButton_clicked
 * The rightmost button supposed to stop the video
 */
void MainWindow::on_stopButton_clicked() {
}

void MainWindow::update_video(QImage frame) {
    using namespace std::chrono;
    milliseconds new_ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    //cout << "QImage arrives at mainwindow: " << (new_ms - mvideo_player->ms).count() << endl;

    ui->videoFrame->setPixmap(QPixmap::fromImage(frame));
}

void MainWindow::set_video_slider_pos(int pos) {
    if (pos <= video_slider->maximum()) {
        video_slider->setSliderPosition(pos);
    }
}

//Used for rescaling the source image for video playback
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   mvideo_player->set_frame_height(ui->videoFrame->height());
   mvideo_player->set_frame_width(ui->videoFrame->width());

}


