#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <QCloseEvent>
#include <chrono>
#include <thread>
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
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    //video_slider = findChild<QSlider*>("videoSlider");
    video_slider = ui->videoSlider;
    iconOnButtonHandler = new IconOnButtonHandler();
    iconOnButtonHandler->set_pictures_to_buttons(ui);

    setShortcuts();

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
 * @brief MainWindow::setShortcuts
 * Function to set shortcuts on actions
 */
void MainWindow::setShortcuts(){
    ui->actionExit->setShortcut(tr("Ctrl+e"));
}

/**
 * @brief MainWindow::setStatusBar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void MainWindow::setStatusBar(string status, int timer = 750){
    ui->statusBar->showMessage(QString::fromStdString(status), timer);
}

/**
 * @brief MainWindow::on_fastBackwardButton_clicked
 * The button supposed to play the video slower
 *
 */
void MainWindow::on_fastBackwardButton_clicked(){
    mvideo_player->dec_playback_speed();
}


/**
 * @brief MainWindow::on_playPauseButton_clicked
 * The button supposed to play and pause the video
 */
void MainWindow::on_playPauseButton_clicked() {
    if (mvideo_player->is_paused() || mvideo_player->is_stopped()) {
        iconOnButtonHandler->setIcon("pause", ui->playPauseButton);//changes the icon on the play button to a pause-icon
        mvideo_player->start();
    } else {
        setStatusBar("Playing");
        iconOnButtonHandler->setIcon("play", ui->playPauseButton);
        mvideo_player->play_pause();
        mvideo_player->wait();
    }
}


/**
 * @brief MainWindow::on_fastForwardButton_clicked
 * The button supposed to play the video faster
 */
void MainWindow::on_fastForwardButton_clicked(){
    mvideo_player->inc_playback_speed();
    setStatusBar("Increased playback speed");
}

/**
 * @brief MainWindow::on_stopButton_clicked
 * The button supposed to stop the video
 */
void MainWindow::on_stopButton_clicked() {
    setStatusBar("Stopped");
    if (!mvideo_player->is_paused()) {
        iconOnButtonHandler->setIcon("play", ui->playPauseButton);
    }

    mvideo_player->stop_video();
}

/**
 * @brief MainWindow::on_nextFrameButton_clicked
 * The button supposed to play the next frame of the video
 */
void MainWindow::on_nextFrameButton_clicked() {
    if (mvideo_player->is_paused()) {
        mvideo_player->next_frame();
    }
}

/**
 * @brief MainWindow::on_nextFrameButton_clicked
 * The button supposed to play the previous frame of the video
 */
void MainWindow::on_previousFrameButton_clicked() {
    if (mvideo_player->is_paused()) {
        mvideo_player->previous_frame();
    }
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
    posCounter++;
    if (pos <= video_slider->maximum() && !slider_blocked) {
        prev_slider_pos = video_slider->value();
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
void MainWindow::on_videoSlider_valueChanged(int newPos){
    changeCounter++;
    // Make slider to follow the mouse directly and not by pageStep steps
    if (abs(prev_slider_pos - newPos) == 1) {
        return;
    } else if (slider_moving) {
        QPoint localMousePos = ui->videoSlider->mapFromGlobal(QCursor::pos());
        float posRatio = localMousePos.x() / (float )ui->videoSlider->size().width();
        int sliderRange = ui->videoSlider->maximum() - ui->videoSlider->minimum();
        int sliderPosUnderMouse = ui->videoSlider->minimum() + sliderRange * posRatio;

        if (!mvideo_player->is_stopped() && !mvideo_player->is_paused()) {
            mvideo_player->stop_video();
        }

        std::chrono::milliseconds current_time = std::chrono::duration_cast<
                std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
        );
        std::chrono::milliseconds time_since_last_slider_frame_update = current_time-slider_timer;
        if (time_since_last_slider_frame_update.count() >= 200) {
            mvideo_player->set_slider_frame(sliderPosUnderMouse);
            slider_timer = current_time;
        }

        return;
    }
    slider_blocked = true;
    Qt::MouseButtons btns = QApplication::mouseButtons();
    QPoint localMousePos = ui->videoSlider->mapFromGlobal(QCursor::pos());
    bool clickOnSlider = (btns & Qt::LeftButton) &&
                         (localMousePos.x() >= 0 && localMousePos.y() >= 0 &&
                          localMousePos.x() < ui->videoSlider->size().width() &&
                          localMousePos.y() < ui->videoSlider->size().height());
    if (clickOnSlider) {
        cout << "Click on slider" << endl;
        // Attention! The following works only for Horizontal, Left-to-right sliders
        float posRatio = localMousePos.x() / (float )ui->videoSlider->size().width();
        int sliderRange = ui->videoSlider->maximum() - ui->videoSlider->minimum();
        int sliderPosUnderMouse = ui->videoSlider->minimum() + sliderRange * posRatio;
        if (sliderPosUnderMouse != newPos) {
            ui->videoSlider->setValue(sliderPosUnderMouse);
            mvideo_player->set_playback_frame(sliderPosUnderMouse);
            return;
        }
    }
    cout << "Done!" << endl;
    slider_blocked = false;
}

/**
 * @brief MainWindow::closeEvent
 * asks if you are sure you want to quit.
 * @param event closing
 */
void MainWindow::closeEvent (QCloseEvent *event){
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                tr("Are you sure you want to quit?\n"),
                                                                QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::No);

    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

/**
 * @brief MainWindow::on_actionExit_triggered
 * sends a closeEvent when you press exit
 */
void MainWindow::on_actionExit_triggered(){
    this->close();
}

/**
 * @brief MainWindow::on_bookmarkButton_clicked
 * the button supposed to add a bookmark
 */
void MainWindow::on_bookmarkButton_clicked(){
    // The code here is only temporary and should be moved/removed
    // once a proper video selector is added
    mvideo_player->load_video("seq_01.mp4");
    iconOnButtonHandler->setIcon("pause", ui->playPauseButton);
    video_slider->setMaximum(mvideo_player->get_num_frames());
    mvideo_player->set_playback_frame(700);
}


/**
 * @brief MainWindow::on_videoSlider_sliderPressed
 * Block slider update from video_player
 */
void MainWindow::on_videoSlider_sliderPressed() {
    slider_blocked = true;
    slider_moving = true;
}

/**
 * @brief MainWindow::on_videoSlider_sliderReleased
 * Set video playback pos to slider pos and unblock slider update
 */
void MainWindow::on_videoSlider_sliderReleased() {
    int new_pos = video_slider->value();
    mvideo_player->set_playback_frame(new_pos);
    video_slider->setSliderPosition(new_pos);
    slider_blocked = false;
    slider_moving = false;
    if (mvideo_player->is_stopped()) {
        mvideo_player->start();
    }
}
