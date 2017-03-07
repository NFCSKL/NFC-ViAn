#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <QCloseEvent>
#include <chrono>
#include <thread>
#include "icononbuttonhandler.h"
#include "inputwindow.h"

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
    video_slider = findChild<QSlider*>("videoSlider");
    iconOnButtonHandler = new IconOnButtonHandler();
    iconOnButtonHandler->set_pictures_to_buttons(ui);

    fileHandler = new FileHandler();
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
    delete fileHandler;
    delete mvideo_player;
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
void MainWindow::on_videoSlider_valueChanged(int newPos){
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

    if (resBtn != QMessageBox::Yes){
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
 * @brief MainWindow::on_actionAddProject_triggered
 */
void MainWindow::on_actionAddProject_triggered() {
    ACTION action = ADD_PROJECT;
    inputWindow = new inputwindow(this, action, "Project name:");
    inputWindow->show();
}

/**
 * @brief MainWindow::inputSwitchCase
 * @param input the input from the user
 * @param action the action that was triggered earlier
 */
void MainWindow::inputSwitchCase(ACTION action, QString qInput) {
    std::string input = qInput.toStdString();
    switch(action){
        case ADD_PROJECT:{
            fileHandler->createProject(input);
            QTreeWidgetItem *projectInTree = new QTreeWidgetItem();
            projectInTree->setText(0, qInput);
            ui->ProjectTree->addTopLevelItem(projectInTree);
            break;
        }
        case CANCELL:
            break;
        default:
            break;

    }
    delete inputWindow;
}

void MainWindow::on_ProjectTree_itemClicked(QTreeWidgetItem *item, int column) {

}
