#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <QCloseEvent>
#include <QColorDialog>
#include <chrono>
#include <thread>
#include "icononbuttonhandler.h"
#include "inputwindow.h"
#include "Video/shapes.h"

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
    set_shortcuts();

    // Add this object as a listener to videoFrame.
    ui->videoFrame->installEventFilter(this);

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
 * @brief MainWindow::set_shortcuts
 * Function to set shortcuts on actions
 */
void MainWindow::set_shortcuts(){
    ui->actionExit->setShortcut(tr("Ctrl+e"));
}

/**
 * @brief MainWindow::set_status_bar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void MainWindow::set_status_bar(string status, int timer = 750){
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
        set_status_bar("Playing");
        iconOnButtonHandler->set_icon("pause", ui->playPauseButton);//changes the icon on the play button to a pause-icon
        mvideo_player->start();
    } else {
        set_status_bar("Paused");
        iconOnButtonHandler->set_icon("play", ui->playPauseButton);
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
    set_status_bar("Stopped");
    if (!mvideo_player->is_paused()) {
        iconOnButtonHandler->set_icon("play", ui->playPauseButton);
    }

    mvideo_player->stop_video();
}

/**
 * @brief MainWindow::on_nextFrameButton_clicked
 * The button supposed to play the next frame of the video
 */
void MainWindow::on_nextFrameButton_clicked() {
    if (mvideo_player->is_paused()) {
        set_status_bar("Went forward a frame");
        mvideo_player->next_frame();
    } else {
        set_status_bar("Needs to be paused");
    }
}

/**
 * @brief MainWindow::on_previousFrameButton_clicked
 * The button supposed to play the previous frame of the video
 */
void MainWindow::on_previousFrameButton_clicked() {
    if (mvideo_player->is_paused()) {
        set_status_bar("Went back a frame");
        mvideo_player->previous_frame();
    } else {
        set_status_bar("Needs to be paused");
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
    set_status_bar("Closing");
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
    iconOnButtonHandler->set_icon("pause", ui->playPauseButton);
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
    set_status_bar("Adding project, need name");
}

/**
 * @brief MainWindow::inputSwitchCase
 * @param input the input from the user
 * @param action the action that was triggered earlier
 */
void MainWindow::inputSwitchCase(ACTION action, QString qInput) {
    std::string input = qInput.toStdString();
    switch(action){
        case ADD_PROJECT: {
            fileHandler->create_project(input);
            QTreeWidgetItem *projectInTree = new QTreeWidgetItem();
            projectInTree->setText(0, qInput);
            ui->ProjectTree->addTopLevelItem(projectInTree);
            set_status_bar("Project " + input + "created.");
            break;
        }
        case CANCEL: {
            set_status_bar("Cancel");
            break;
        }
        default:
            break;

    }
    delete inputWindow;
}
/**
 * @brief MainWindow::on_ProjectTree_itemClicked
 * @param item the item in the projectTree that was clicked
 * @param column the column in the tree
 */
void MainWindow::on_ProjectTree_itemClicked(QTreeWidgetItem *item, int column) {
}

 /** @brief MainWindow::on_actionShow_hide_overview_triggered
 * Toggles the showing/hiding of the overlay.
 * Invoked by menu item.
 */
void MainWindow::on_actionShow_hide_overview_triggered() {
    mvideo_player->toggle_overlay();
}

/**
 * @brief MainWindow::on_actionColour_triggered
 * Selects a colour for the overlay drawing tool.
 */
void MainWindow::on_actionColour_triggered() {
    QColor col = QColorDialog::getColor();
    mvideo_player->set_overlay_colour(col);
}

/**
 * @brief MainWindow::on_actionRectangle_triggered
 * Selects the rectangle shape for the overlay drawing tool.
 */
void MainWindow::on_actionRectangle_triggered() {
    mvideo_player->set_overlay_tool(RECTANGLE);
}

/**
 * @brief MainWindow::on_actionCircle_triggered
 * Selects the circle shape for the overlay drawing tool.
 */
void MainWindow::on_actionCircle_triggered() {
    mvideo_player->set_overlay_tool(CIRCLE);
}

/**
 * @brief MainWindow::on_actionLine_triggered
 * Selects the line shape for the overlay drawing tool.
 */
void MainWindow::on_actionLine_triggered() {
    mvideo_player->set_overlay_tool(LINE);
}

/**
 * @brief MainWindow::on_actionArrow_triggered
 * Selects the arrow shape for the overlay drawing tool.
 */
void MainWindow::on_actionArrow_triggered() {
    mvideo_player->set_overlay_tool(ARROW);
}

/**
 * @brief MainWindow::eventFilter
 * Listener function for all eventFilters MainWindow has installed.
 * @param obj the object invoking the event
 * @param event the invooked event
 * @return Returns true if the event matched any of the overlay's
 *         functionality, else false.
 *         (Returning false means that the event is sent to the
 *          target object instead, but not if true is returned.)
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // Check who invoked the event.
    if (qobject_cast<QLabel*>(obj)==ui->videoFrame) {
        // Cast to a mouse event to get the mouse position.
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint pos = mouseEvent->pos();
        // Check what kind of event.
        if (event->type() == QEvent::MouseButtonPress) {
            mvideo_player->video_mouse_pressed(pos);
            return true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            mvideo_player->video_mouse_released(pos);
            return true;
        } else if (event->type() == QEvent::MouseMove) {
            mvideo_player->video_mouse_moved(pos);
            return true;
        }
    }
    return false;
}
