#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>
#include <thread>
#include <iostream>
#include "icononbuttonhandler.h"

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
    iconOnButtonHandler = new IconOnButtonHandler();
    iconOnButtonHandler->set_pictures_to_buttons(ui);

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

    delete iconOnButtonHandler;
    delete ui;
}

/**
 * @brief MainWindow::on_playButton_clicked
 * The button supposed to play the video
 */
void MainWindow::on_playButton_clicked() {
    if (mvideo_player->is_paused()) {
        iconOnButtonHandler->setIcon("pause", ui->playButton);//changes the icon on the play button to a pause-icon
        mvideo_player->play_pause();
        mvideo_player->start();
    } else {
        iconOnButtonHandler->setIcon("play", ui->playButton);
        mvideo_player->play_pause();
        mvideo_player->wait();
    }
}

/**
 * @brief MainWindow::on_pauseButton_clicked
 * The button supposed to pause the video
 */
void MainWindow::on_pauseButton_clicked() {
    // The code here is only temporary and should be moved/removed
    // once a proper video selector is added
    mvideo_player->load_video("seq_01.mp4");
    iconOnButtonHandler->setIcon("pause", ui->playButton);
    video_slider->setMaximum(mvideo_player->get_num_frames());
}


/**
 * @brief MainWindow::on_stopButton_clicked
 * The button supposed to stop the video
 */
void MainWindow::on_stopButton_clicked() {
    if (!mvideo_player->is_paused()) {
        mvideo_player->play_pause();
        iconOnButtonHandler->setIcon("play", ui->playButton);
    }

    mvideo_player->set_playback_frame(0);
    set_video_slider_pos(0);
}

/**
 * @brief MainWindow::update_video
 * Sets the videoFrame pixmap to the current frame from video
 * @param frame
 */
void MainWindow::update_video(QImage frame) {

    ui->videoFrame->setPixmap(QPixmap::fromImage(frame));
}

/**
 * @brief MainWindow::set_video_slider_pos
 * Sets the position of slider in video to position pos
 * @param pos
 */
void MainWindow::set_video_slider_pos(int pos) {
    if (pos <= video_slider->maximum()) {
        video_slider->setSliderPosition(pos);
    }
}

/**
 * @brief MainWindow::resizeEvent
 * Used for rescaling the source image for video playback
 * @param event
 */
void MainWindow::resizeEvent(QResizeEvent* event) {
   QMainWindow::resizeEvent(event);
   mvideo_player->set_frame_height(ui->videoFrame->height());
   mvideo_player->set_frame_width(ui->videoFrame->width());
}

/**
 * @brief MainWindow::on_videoSlider_valueChanged
 * Update the slider to where the mouse is
 * @param newPos current position of the slider
 */
void MainWindow::on_videoSlider_valueChanged(int newPos)
{
    // Make slider to follow the mouse directly and not by pageStep steps
    Qt::MouseButtons btns = QApplication::mouseButtons();
    QPoint localMousePos = ui->videoSlider->mapFromGlobal(QCursor::pos());
    bool clickOnSlider = (btns & Qt::LeftButton) &&
                         (localMousePos.x() >= 0 && localMousePos.y() >= 0 &&
                          localMousePos.x() < ui->videoSlider->size().width() &&
                          localMousePos.y() < ui->videoSlider->size().height());
    if (clickOnSlider)
    {
        // Attention! The following works only for Horizontal, Left-to-right sliders
        float posRatio = localMousePos.x() / (float )ui->videoSlider->size().width();
        int sliderRange = ui->videoSlider->maximum() - ui->videoSlider->minimum();
        int sliderPosUnderMouse = ui->videoSlider->minimum() + sliderRange * posRatio;
        if (sliderPosUnderMouse != newPos)
        {
            ui->videoSlider->setValue(sliderPosUnderMouse);
            return;
        }
    }
}
