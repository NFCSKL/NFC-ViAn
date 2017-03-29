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
#include "Video/shapes/shape.h"

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

    this->selectedProject = nullptr;
    this->selectedVideo = nullptr;

    video_slider = findChild<QSlider*>("videoSlider");
    iconOnButtonHandler = new IconOnButtonHandler();
    iconOnButtonHandler->set_pictures_to_buttons(ui);

    fileHandler = new FileHandler();
    set_shortcuts();

    // Add this object as a listener to videoFrame.
    ui->videoFrame->installEventFilter(this);

    ui->ProjectTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ProjectTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::prepare_menu);

    mvideo_player = new video_player();
    QObject::connect(mvideo_player, SIGNAL(processedImage(QImage)),
                                  this, SLOT(update_video(QImage)));
    QObject::connect(mvideo_player, SIGNAL(currentFrame(int)),
                                  this, SLOT(set_video_slider_pos(int)));

    //Used for rescaling the source image for video playback
    mvideo_player->set_frame_height(ui->videoFrame->height());
    mvideo_player->set_frame_width(ui->videoFrame->width());

    // Initially hide overlay toolbar
    ui->toolBar->hide();
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
    ui->actionSave->setShortcut(tr("Ctrl+s"));
    ui->actionAddProject->setShortcut(tr("Ctrl+Shift+p"));
    ui->actionAddVideo->setShortcut(tr("Ctrl+Shift+v"));
}

/**
 * @brief MainWindow::set_status_bar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void MainWindow::set_status_bar(string status, int timer){
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
void MainWindow::input_switch_case(ACTION action, QString qInput) {
    std::string input = qInput.toStdString();
    switch(action){
        case ADD_PROJECT: {
            Project* proj = fileHandler->create_project(input);
            add_project_to_tree(proj);
            set_status_bar("Project " + input + " created.");
            delete inputWindow;
            break;
        }
        case CANCEL: {
            set_status_bar("Cancel");
            delete inputWindow;
            break;
        }
        case ADD_VIDEO: {
            add_video_to_tree(selectedProject, input);
            set_status_bar("Video " + input + " added.");
            break;
        }
        default:
            break;

    }
}

/**
 * @brief MainWindow::on_ProjectTree_itemClicked
 * @param item the item in the projectTree that was clicked
 * @param column the column in the tree
 * If you click on the selected video it will start playing.
 */
void MainWindow::on_ProjectTree_itemClicked(QTreeWidgetItem *item, int column) {
    MyQTreeWidgetItem *q_item = (MyQTreeWidgetItem*)item;
    switch(q_item->type) {
    case TYPE::PROJECT:
        set_selected_project(q_item);
        break;
    case TYPE::VIDEO:
        if(q_item == selectedVideo) {
            play_video();
        } else {
            set_selected_video(q_item);
        }
        break;
    default:
        break;
    }
}

 /** @brief MainWindow::on_actionShow_hide_overview_triggered
 * Toggles the showing/hiding of the overlay.
 * Invoked by menu item.
 */
void MainWindow::on_actionShow_hide_overview_triggered() {
    mvideo_player->toggle_overlay();
    toggle_toolbar();
    if (mvideo_player->is_showing_overlay()) {
        set_status_bar("Overlay: On.");
    } else {
        set_status_bar("Overlay: Off.");
    }
}

/**
 * @brief MainWindow::on_actionColour_triggered
 * Selects a colour for the overlay drawing tool.
 */
void MainWindow::on_actionColour_triggered() {
    QColor col = QColorDialog::getColor();
    mvideo_player->set_overlay_colour(col);
    string msg = "Color: ";
    msg.append(col.name().toStdString());
    set_status_bar(msg);
}

/**
 * @brief MainWindow::on_actionRectangle_triggered
 * Selects the rectangle shape for the overlay drawing tool.
 */
void MainWindow::on_actionRectangle_triggered() {
    mvideo_player->set_overlay_tool(RECTANGLE);
    set_status_bar("Tool: rectangle.");
}

/**
 * @brief MainWindow::on_actionCircle_triggered
 * Selects the circle shape for the overlay drawing tool.
 */
void MainWindow::on_actionCircle_triggered() {
    mvideo_player->set_overlay_tool(CIRCLE);
    set_status_bar("Tool: circle.");
}

/**
 * @brief MainWindow::on_actionLine_triggered
 * Selects the line shape for the overlay drawing tool.
 */
void MainWindow::on_actionLine_triggered() {
    mvideo_player->set_overlay_tool(LINE);
    set_status_bar("Tool: line.");
}

/**
 * @brief MainWindow::on_actionArrow_triggered
 * Selects the arrow shape for the overlay drawing tool.
 */
void MainWindow::on_actionArrow_triggered() {
    mvideo_player->set_overlay_tool(ARROW);
    set_status_bar("Tool: arrow.");
}

/**
 * @brief MainWindow::on_actionPen_triggered
 * Selects the pen for the overlay drawing tool.
 */
void MainWindow::on_actionPen_triggered() {
    mvideo_player->set_overlay_tool(PEN);
    set_status_bar("Tool: pen.");
}

/**
 * @brief MainWindow::on_actionText_triggered
 * Selects the text for the overlay drawing tool.
 */
void MainWindow::on_actionText_triggered() {
    mvideo_player->set_overlay_tool(TEXT);
    set_status_bar("Tool: text.");
}

/**
 * @brief MainWindow::on_actionUndo_triggered
 * Undo the drawings on the overlay.
 */
void MainWindow::on_actionUndo_triggered() {
    mvideo_player->undo_overlay();
    set_status_bar("Undo drawing.");
}

/**
 * @brief MainWindow::on_actionClear_triggered
 * Clear the drawings on the overlay.
 */
void MainWindow::on_actionClear_triggered() {
    mvideo_player->clear_overlay();
    set_status_bar("Cleared drawings.");
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

/**
 * @brief MainWindow::on_actionZoom_in_triggered
 * Sets a state in the video overlay
 * for the user to choose an area.
 */
void MainWindow::on_actionZoom_in_triggered() {
    mvideo_player->zoom_in();
    set_status_bar("Zoom in. Coose your area.");
}

/**
 * @brief MainWindow::on_actionZoom_out_triggered
 * Reset the zoom level to the full video size.
 */
void MainWindow::on_actionZoom_out_triggered() {
    mvideo_player->zoom_out();
    set_status_bar("Zoom out.");
}

/**
 * @brief MainWindow::prepare_menu
 * @param pos
 * Creates context menu on right-click in tree view
 */
void MainWindow::prepare_menu(const QPoint & pos) {
    QTreeWidget *tree = ui->ProjectTree;

    MyQTreeWidgetItem *item = (MyQTreeWidgetItem*)tree->itemAt( pos );

    QMenu menu(this);

    if(item == nullptr) {
        QAction *create_project = new QAction(QIcon(""), tr("&Create project"), this);
        QAction *load_project = new QAction(QIcon(""), tr("&Load project"), this);
        create_project->setStatusTip(tr("Create project"));
        load_project->setStatusTip(tr("Load project"));
        menu.addAction(create_project);
        menu.addAction(load_project);
        connect(create_project, SIGNAL(triggered()), this, SLOT(on_actionAddProject_triggered()));
        connect(load_project, SIGNAL(triggered()), this, SLOT(on_actionLoad_triggered()));
    } else if(item->type == TYPE::PROJECT) {
        set_selected_project(item);
        QAction *add_video = new QAction(QIcon(""), tr("&Add video"), this);
        QAction *delete_project = new QAction(QIcon(""), tr("&Delete project"), this);
        add_video->setStatusTip(tr("Add video"));
        delete_project->setStatusTip(tr("Delete project"));
        menu.addAction(add_video);
        menu.addAction(delete_project);
        connect(add_video, SIGNAL(triggered()), this, SLOT(on_actionAddVideo_triggered()));
        connect(delete_project, SIGNAL(triggered()), this, SLOT(on_actionDeleteProject_triggered()));
    } else if(item->type == TYPE::VIDEO) {
        set_selected_video(item);
        QAction *load_video = new QAction(QIcon(""), tr("&Play video"), this);
        load_video->setStatusTip(tr("Play video"));
        menu.addAction(load_video);
        connect(load_video, SIGNAL(triggered()), this, SLOT(play_video()));
    }
    QPoint pt(pos);
    menu.exec( tree->mapToGlobal(pos) );
}

/**
 * @brief MainWindow::on_actionAddVideo_triggered
 * Prompts user with file browser to add video
 * to selected project
 */
void MainWindow::on_actionAddVideo_triggered() {
    if(selectedProject != nullptr) {
        QString dir = QFileDialog::getOpenFileName(this, tr("Choose video"), WORKSPACE,
                                                   tr("Videos (*.avi *.mkv *.mov *.mp4 *.3gp *.flv *.webm *.ogv *.m4v)"));
        if(!dir.isEmpty()) { // Check if you have selected something.
            input_switch_case(ACTION::ADD_VIDEO, dir);
        }
    } else {
        set_status_bar("No project selected.");
    }
}

/**
 * @brief MainWindow::play_video
 *  Loads selected video, flips playbutton to pause
 *  plays video from beginning
 *
 */
void MainWindow::play_video() {
    enable_video_buttons();
    mvideo_player->load_video(selectedVideo->name.toStdString());
    iconOnButtonHandler->set_icon("pause", ui->playPauseButton);
    video_slider->setMaximum(mvideo_player->get_num_frames());
    mvideo_player->set_playback_frame(0);
}

/**
 * @brief MainWindow::set_selected_project
 * puts an arrow pointing at the selected project
 * @param newSelectedProject
 */
void MainWindow::set_selected_project(MyQTreeWidgetItem *newSelectedProject){
    set_selected(selectedProject, newSelectedProject);
}

/**
 * @brief MainWindow::set_selected_video
 * puts an arrow pointing at the selected video
 * @param newSelectedVideo
 */
void MainWindow::set_selected_video(MyQTreeWidgetItem *newSelectedVideo) {
    set_selected(selectedVideo, newSelectedVideo);
    set_selected_project((MyQTreeWidgetItem*)selectedVideo->parent());
}

/**
 * @brief MainWindow::set_selected
 * @param selected old
 * @param new_selected
 */
void MainWindow::set_selected(MyQTreeWidgetItem *&selected, MyQTreeWidgetItem *new_selected) {
    if(selected == nullptr) {
        selected = new_selected;
        QString string = selected->text(0);
        string.append(QString::fromStdString(ARROW_STRING));
        selected->setText(0, string);
    } else if (selected != new_selected) {
        QString string = selected->text(0);
        string.chop(ARROW_STRING.length());
        selected->setText(0, string);
        selected = new_selected;
        string = selected->text(0);
        string.append(QString::fromStdString(ARROW_STRING));
        selected->setText(0, string);
    }
}

/**
 * @brief MainWindow::on_actionSave_triggered
 * saves project which is selected in tree view,
 * checks if there is one
 */
void MainWindow::on_actionSave_triggered() {
    if(selectedProject != nullptr) {
        this->fileHandler->save_project(this->selectedProject->id);
        std::string text = "Saved project " + this->selectedProject->name.toStdString();
        set_status_bar(text);
    } else {
        set_status_bar("Nothing to save");
    }
}

/**
 * @brief MainWindow::on_actionLoad_triggered
 */
void MainWindow::on_actionLoad_triggered() {
    QString dir = QFileDialog::getOpenFileName(this, tr("Choose project"),"C:/",tr("*.txt"));
    if(!dir.isEmpty()) { // Check if you have selected something.
        Project* loadProj= this->fileHandler->load_project(dir.toStdString());
        add_project_to_tree(loadProj);
        set_status_bar("Project " + loadProj->m_name + " loaded.");
    }
}

/**
 * @brief MainWindow::add_project_to_tree
 * @param proj to add to tree
 * also adds all videos of the project to the tree
 */
void MainWindow::add_project_to_tree(Project* proj) {
    MyQTreeWidgetItem *projectInTree = new MyQTreeWidgetItem(TYPE::PROJECT, QString::fromStdString(proj->m_name), proj->m_id);
    projectInTree->setText(0, QString::fromStdString(proj->m_name));
    set_selected_project(projectInTree);
    ui->ProjectTree->addTopLevelItem(projectInTree);
    for(Video *v: proj->m_videos) {
        std::stringstream filePath;
        filePath << *v;
        std::string treeName = filePath.str();
        add_video_to_tree(projectInTree, treeName);
    }
}

/**
 * @brief MainWindow::add_video_to_tree
 * @param project to add videos to
 * @param filePath of the video
 */
void MainWindow::add_video_to_tree(MyQTreeWidgetItem *project, std::string filePath) {
    fileHandler->add_video(fileHandler->get_project(project->id), filePath);
    MyQTreeWidgetItem *videoInTree = new MyQTreeWidgetItem(TYPE::VIDEO, QString::fromStdString(filePath));
    videoInTree->set_text_from_filepath(filePath);
    project->addChild(videoInTree);
    set_selected_video(videoInTree);
}

/**
 * @brief MainWindow::on_actionDeleteProject_triggered
 * Deletes the saved files of the selected project.
 * Removes the project from the preoject tree.
 */
void MainWindow::on_actionDeleteProject_triggered() {
    if(selectedProject != nullptr) {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Delete",
                                                                    tr("Are you sure you want to delete the selected project?\n"),
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::No);

        if (resBtn == QMessageBox::Yes) {
            this->fileHandler->delete_project(fileHandler->get_project(this->selectedProject->id));
            remove_selected_project_from_tree();
        }
    } else {
        set_status_bar("No selected project to remove.");
    }
}

/**
 * @brief MainWindow::remove_selected_project_from_tree
 * Removes all videos of the selected project and then the project.
 */
void MainWindow::remove_selected_project_from_tree() {
    for(int child_number = 0; child_number < selectedProject->childCount(); child_number++) {
        remove_video_from_tree((MyQTreeWidgetItem*)selectedProject->child(child_number));
    }
    ui->ProjectTree->removeItemWidget(selectedProject, 0);
    delete selectedProject;
    selectedProject = nullptr;
}

/**
 * @brief MainWindow::remove_selected_video_from_tree
 * @param video to be deleted
 * Removes the video from the tree.
 */
void MainWindow::remove_video_from_tree(MyQTreeWidgetItem *video) {
    if (video == selectedVideo) {
        selectedVideo = nullptr;
    }
    ui->ProjectTree->removeItemWidget(video, 0);
    delete video;
}

/**
 * @brief MainWindow::toggle_toolbar
 * This method will toggle the toolbar depending on wether the overlay is showing or not.
 * It is switching between a toolbar that contains items as save/add/load and another that
 * contains drawing tools.
 * This is invoked when the overlay is activated and deactivated.
 */
void MainWindow::toggle_toolbar() {
    if(mvideo_player->is_showing_overlay()) {
        ui->toolBar_no_overlay->hide();
        ui->toolBar->show();
    }else {
        ui->toolBar->hide();
        ui->toolBar_no_overlay->show();
    }
}

/**
 * @brief MainWindow::enable_video_buttons
 * Enable the videobuttons.
 * They are disabled as default.
 */
void MainWindow::enable_video_buttons() {
    ui->nextFrameButton->setEnabled(true);
    ui->fastBackwardButton->setEnabled(true);
    ui->playPauseButton->setEnabled(true);
    ui->fastForwardButton->setEnabled(true);
    ui->previousFrameButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
}
