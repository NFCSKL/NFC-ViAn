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

    video_slider = findChild<QSlider*>("videoSlider");
    iconOnButtonHandler = new IconOnButtonHandler();
    iconOnButtonHandler->set_pictures_to_buttons(ui);

    // Setup a Bookmark View in the right sidebar in the GUI.
    bookmark_view = new BookmarkView(ui->documentList);

    fileHandler = new FileHandler();
    set_shortcuts();

    // Add this object as a listener to videoFrame.
    ui->videoFrame->installEventFilter(this);
    ui->videoFrame->setScaledContents(false);

    ui->ProjectTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ProjectTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::prepare_menu);

    //Creates and prepares the video_player.
    mvideo_player = new video_player(&mutex, &paused_wait);
    setup_video_player(mvideo_player);

    // Initially hide overlay and analysis toolbar
    ui->toolBar_overlay->hide();
    ui->toolBar_analysis->hide();
}

/**
 * @brief MainWindow::~MainWindow
 * Destructor
 */
MainWindow::~MainWindow() {

    delete iconOnButtonHandler;
    delete fileHandler;
    mvideo_player->terminate();
    delete mvideo_player;
    delete ui;
    delete bookmark_view;
}

/**
 * @brief MainWindow::setup_video_player
 * @param mplayer
 * Connects all signals and slots that are needed between video_player and mainwindow.
 */
void MainWindow::setup_video_player(video_player *mplayer) {
    QObject::connect(mplayer, SIGNAL(processed_image(QImage)),
                     this, SLOT(update_video(QImage)));
    QObject::connect(mplayer, SIGNAL(update_current_frame(int)),
                     this, SLOT(set_video_slider_pos(int)));
    QObject::connect(this, SIGNAL(resize_video_frame(int,int)),
                     mplayer, SLOT(scaling_event(int,int)));
    QObject::connect(this, SIGNAL(next_video_frame()),
                     mplayer, SLOT(next_frame()));
    QObject::connect(this, SIGNAL(prev_video_frame()),
                     mplayer, SLOT(previous_frame()));
    QObject::connect(this, SIGNAL(set_play_video()),
                     mplayer, SLOT(on_play_video()));
    QObject::connect(this, SIGNAL(set_pause_video()),
                     mplayer, SLOT(on_pause_video()));
    QObject::connect(this, SIGNAL(set_stop_video()),
                     mplayer, SLOT(on_stop_video()));
}

/**
 * @brief MainWindow::set_shortcuts
 * Function to set keyboard shortcuts on actions
 */
void MainWindow::set_shortcuts(){
    ui->actionExit->setShortcut(tr("Ctrl+e"));
    ui->actionSave->setShortcut(tr("Ctrl+s"));
    ui->actionLoad->setShortcut(tr("Ctrl+l"));
    ui->actionAddProject->setShortcut(tr("Ctrl+Shift+p"));
    ui->actionAddVideo->setShortcut(tr("Ctrl+Shift+v"));
}

/**
 * @brief MainWindow::set_status_bar
 * @param status text to show in the statusbar
 * @param timer time to show it in the bar in ms, 750ms is standard
 */
void MainWindow::set_status_bar(std::string status, int timer){
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
    if (mvideo_player->is_paused()) {
        // Video thread is paused. Notifying the waitcondition to resume playback
        set_status_bar("Playing");
        iconOnButtonHandler->set_icon("pause", ui->playPauseButton);//changes the icon on the play button to a pause-icon
        paused_wait.wakeOne();
    } else if (mvideo_player->is_stopped()) {
        // Video thread has finished. Start a new one
        iconOnButtonHandler->set_icon("pause", ui->playPauseButton);
        mvideo_player->start();
    } else {
        // Video thread is running. Pause it
        set_status_bar("Paused");
        iconOnButtonHandler->set_icon("play", ui->playPauseButton);
        emit set_pause_video();
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
    } else {
        paused_wait.wakeOne();
    }
    emit set_stop_video();
}

/**
 * @brief MainWindow::on_nextFrameButton_clicked
 * The button supposed to play the next frame of the video
 */
void MainWindow::on_nextFrameButton_clicked() {
    if (mvideo_player->is_paused()) {
        set_status_bar("Went forward a frame");
        emit next_video_frame();
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
        emit prev_video_frame();
    } else {
        set_status_bar("Video needs to be paused");
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

   //Scales the current frame when video playback is paused
   if (mvideo_player->video_open() && mvideo_player->is_paused()) {
       QImage frame( ui->videoFrame->pixmap()->toImage() );
       ui->videoFrame->setPixmap(QPixmap::fromImage(
                                     frame.scaled(ui->videoFrame->width(),
                                                  ui->videoFrame->height(),
                                                  Qt::KeepAspectRatio))
                                 );
   }

   //Sends new QLabel resolution to mvideo_player to update scaling resolution
   emit resize_video_frame(ui->videoFrame->width(), ui->videoFrame->height());
}

/**
 * @brief MainWindow::on_videoSlider_valueChanged
 * Update the slider to where the mouse is
 * @param newPos current position of the slider
 */
void MainWindow::on_videoSlider_valueChanged(int newPos) {
    // Make slider to follow the mouse directly and not by pageStep steps
    Qt::MouseButtons btns = QApplication::mouseButtons();
    QPoint localMousePos = ui->videoSlider->mapFromGlobal(QCursor::pos());
    bool clickOnSlider = (btns & Qt::LeftButton) &&
                         (localMousePos.x() >= 0 && localMousePos.y() >= 0 &&
                          localMousePos.x() < ui->videoSlider->size().width() &&
                          localMousePos.y() < ui->videoSlider->size().height());
    if (clickOnSlider) {
        // Attention! The following works only for Horizontal, Left-to-right sliders
        float posRatio = localMousePos.x() / (float )ui->videoSlider->size().width();
        int sliderRange = ui->videoSlider->maximum() - ui->videoSlider->minimum();
        int sliderPosUnderMouse = ui->videoSlider->minimum() + sliderRange * posRatio;
        if (sliderPosUnderMouse != newPos) {
            ui->videoSlider->setValue(sliderPosUnderMouse);
            return;
        }
    }
}

/**
 * @brief MainWindow::closeEvent
 * asks if you are sure you want to quit.
 * TODO Needs to close all other threads before exiting the program
 * @param event closing
 */
void MainWindow::closeEvent (QCloseEvent *event) {
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
void MainWindow::on_actionExit_triggered() {
    this->close();
}

/**
 * @brief MainWindow::on_bookmarkButton_clicked
 * Button to add a bookmark to the bookmark view.
 */
void MainWindow::on_bookmarkButton_clicked() {
    QTreeWidgetItem *item;
    MyQTreeWidgetItem *my_project;
    if(ui->ProjectTree->selectedItems().size() == 1) {
        // Get current project.
        item = ui->ProjectTree->selectedItems().first();
        my_project = (MyQTreeWidgetItem*)get_project_from_object(item);
        std::string proj_path = fileHandler->get_dir(my_project->id);
        // Add bookmarks-folder to the project-folder.
        proj_path.append("/bookmarks");
        ID dir_id = fileHandler->create_directory(proj_path);
        std::string dir_path = fileHandler->get_dir(dir_id);

        // Export the current frame in the bookmarks-folder.
        // The names of the stored files will have increasing numbers.
        std::string file_name = std::to_string(bookmark_view->get_num_bookmarks());
        std::string file_path = mvideo_player->export_current_frame(dir_path, file_name);

        bookmark_view->add_bookmark(file_path);
        set_status_bar("Saved bookmark.");
    }
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
    MyQTreeWidgetItem *my_project;
    if(ui->ProjectTree->selectedItems().size() == 1) {
        my_project = (MyQTreeWidgetItem*)ui->ProjectTree->selectedItems().first();
    } else {
        set_status_bar("Multiple or no things selected.");
    }
    switch(action) {
    case ADD_PROJECT: {
        Project* proj = fileHandler->create_project(input);
        add_project_to_tree(proj);
        set_status_bar("Project " + input + " created.");
        delete inputWindow;
        break;
    }
    case CANCEL:
        set_status_bar("Cancel");
        delete inputWindow;
        break;
    case ADD_VIDEO: {
        ID id = fileHandler->add_video(fileHandler->get_project(my_project->id), input);
        add_video_to_tree(input, id);
        set_status_bar("Video " + input + " added.");
        break;
    }
    default:
        break;
    }
}

/**
 * @brief MainWindow::on_ProjectTree_itemDoubleClicked
 * @param item, the item in the projectTree that was clicked
 * @param column, the column in the tree
 * Double clicking on a video will start to play it.
 * Double clicking on a project will expand or collapse it.
 */
void MainWindow::on_ProjectTree_itemDoubleClicked(QTreeWidgetItem *item, int column) {
    MyQTreeWidgetItem *q_item = (MyQTreeWidgetItem*)item;
    switch(q_item->type) {
    case TYPE::PROJECT:
        if (item->isExpanded()) {
            item->setExpanded(true);
        } else {
            item->setExpanded(false);
        }
        break;
    case TYPE::VIDEO:
        play_video();
        break;
    default:
        break;
    }
}

 /** @brief MainWindow::on_actionShow_hide_overlay_triggered
 * Toggles the showing/hiding of the overlay.
 * Invoked by menu item.
 */
void MainWindow::on_actionShow_hide_overlay_triggered() {
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
    if (col.isValid()) {
        mvideo_player->set_overlay_colour(col);
        std::string msg = "Color: ";
        msg.append(col.name().toStdString());
        set_status_bar(msg);
    }
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
    set_status_bar("Zoom in. Choose your area.");
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
        QAction *add_video = new QAction(QIcon(""), tr("&Add video"), this);
        QAction *delete_project = new QAction(QIcon(""), tr("&Delete project"), this);
        add_video->setStatusTip(tr("Add video"));
        delete_project->setStatusTip(tr("Delete project"));
        menu.addAction(add_video);
        menu.addAction(delete_project);
        connect(add_video, SIGNAL(triggered()), this, SLOT(on_actionAddVideo_triggered()));
        connect(delete_project, SIGNAL(triggered()), this, SLOT(on_actionDelete_triggered()));
    } else if(item->type == TYPE::VIDEO) {
        QAction *load_video = new QAction(QIcon(""), tr("&Play video"), this);
        QAction *delete_video = new QAction(QIcon(""), tr("&Remove video"), this);
        load_video->setStatusTip(tr("Play video"));
        delete_video->setStatusTip(tr("Remove video from project"));
        menu.addAction(load_video);
        menu.addAction(delete_video);
        connect(load_video, SIGNAL(triggered()), this, SLOT(play_video()));
        connect(delete_video, SIGNAL(triggered()), this, SLOT(on_actionDelete_triggered()));
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
    QTreeWidgetItem *project;
    if(ui->ProjectTree->selectedItems().size() == 1) {
        project = ui->ProjectTree->selectedItems().first();
        if (((MyQTreeWidgetItem*)project)->type == TYPE::PROJECT){
            QString dir = QFileDialog::getOpenFileName(this, tr("Choose video"), this->fileHandler->work_space.c_str(),
                                                       tr("Videos (*.avi *.mkv *.mov *.mp4 *.3gp *.flv *.webm *.ogv *.m4v)"));
            if(!dir.isEmpty()) { // Check if you have selected something.
                input_switch_case(ACTION::ADD_VIDEO, dir);
            }
        } else {
            set_status_bar("No project selected.");
        }
    } else {
        set_status_bar("Multiple or no projects selected.");
    }
}

/**
 * @brief MainWindow::play_video
 *  Loads selected video, flips playbutton to pause
 *  plays video from beginning
 */
void MainWindow::play_video() {
    MyQTreeWidgetItem *my_project;
    my_project = (MyQTreeWidgetItem*)ui->ProjectTree->selectedItems().first();
    mvideo_player->load_video(my_project->name.toStdString());
    //Used for rescaling the source image for video playback
    emit resize_video_frame(ui->videoFrame->width(),ui->videoFrame->height());
    enable_video_buttons();
    iconOnButtonHandler->set_icon("pause", ui->playPauseButton);
    video_slider->setMaximum(mvideo_player->get_num_frames());
}

/**
 * @brief MainWindow::on_actionSave_triggered
 * saves project which is selected in tree view,
 * checks if there is one
 */
void MainWindow::on_actionSave_triggered() {
    QTreeWidgetItem *item;
    MyQTreeWidgetItem *my_project;
    if(ui->ProjectTree->selectedItems().size() == 1) {
        item = ui->ProjectTree->selectedItems().first();
        my_project = (MyQTreeWidgetItem*)get_project_from_object(item);
        this->fileHandler->save_project(my_project->id);
        std::string text = "Saved project " + my_project->name.toStdString();
        set_status_bar(text);
    } else {
        set_status_bar("Nothing to save");
    }
}

/**
 * @brief MainWindow::on_actionLoad_triggered
 */
void MainWindow::on_actionLoad_triggered() {
    QString dir = QFileDialog::getOpenFileName(this, tr("Choose project"),this->fileHandler->work_space.c_str(),tr("*.txt"));
    if(!dir.isEmpty()) { // Check if you have selected something.
        Project* loadProj= this->fileHandler->load_project(dir.toStdString());
        add_project_to_tree(loadProj);
        set_status_bar("Project " + loadProj->name + " loaded.");
    }
}

/**
 * @brief MainWindow::add_project_to_tree
 * @param proj to add to tree
 * also adds all videos of the project to the tree
 */
void MainWindow::add_project_to_tree(Project* proj) {
    MyQTreeWidgetItem *project_in_tree = new MyQTreeWidgetItem(TYPE::PROJECT, QString::fromStdString(proj->name), proj->id);
    project_in_tree->setText(0, QString::fromStdString(proj->name));
    ui->ProjectTree->addTopLevelItem(project_in_tree);
    ui->ProjectTree->clearSelection();
    project_in_tree->setSelected(true);
    for(auto vid = proj->videos.begin(); vid != proj->videos.end(); ++vid){
        stringstream file_path;
        Video* v = vid->second;
        file_path << *v;
        std::string tree_name = file_path.str();
        add_video_to_tree(tree_name, v->id);
    }
}

/**
 * @brief MainWindow::add_video_to_tree
 * @param file_path of the video
 */
void MainWindow::add_video_to_tree(std::string file_path, ID id) {
    QTreeWidgetItem *project;
    project = ui->ProjectTree->selectedItems().first();
    MyQTreeWidgetItem *video_in_tree = new MyQTreeWidgetItem(TYPE::VIDEO, QString::fromStdString(file_path), id);
    video_in_tree->set_text_from_filepath(file_path);
    project->addChild(video_in_tree);
    project->setExpanded(true);
}

/**
 * @brief MainWindow::on_actionChoose_Workspace_triggered
 * Opens file explorer and requests a workspace select from user, updates
 * filehandler workspace accordingly.
 */
void MainWindow::on_actionChoose_Workspace_triggered() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Workspace"),this->fileHandler->work_space.c_str());
    this->fileHandler->set_workspace(dir.toStdString() + "/");
    set_status_bar("New wokspace set to " + this->fileHandler->work_space);
}

/**
 * @brief MainWindow::on_actionDelete_triggered
 * Deletes the selected item in the tree
 */
void MainWindow::on_actionDelete_triggered() {
    QTreeWidgetItem *item;
    MyQTreeWidgetItem *my_item;
    MyQTreeWidgetItem *my_project;
    if(ui->ProjectTree->selectedItems().size() == 1) {
        item = ui->ProjectTree->selectedItems().first();
        my_item = (MyQTreeWidgetItem*)item;
        QMessageBox::StandardButton res_btn = QMessageBox::question( this, "Delete",
                                                                    tr(("Are you sure you want to delete " + my_item->get_name() + "?\n").c_str()),
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::No);

        if (res_btn == QMessageBox::Yes) {
            if (my_item->type == TYPE::VIDEO) {
                my_project = (MyQTreeWidgetItem*) get_project_from_object(item);
                this->fileHandler->remove_video_from_project(my_project->id, my_item->id); // Remove video from project
            } else if (my_item->type == TYPE::PROJECT) {
                this->fileHandler->delete_project(my_item->id);
            }
            remove_item_from_tree(my_item);
        }
    } else {
        set_status_bar("Multiple or no videos selected.");
    }
}

/**
 * @brief MainWindow::remove_item_from_tree
 * @param my_item item to be removed from tree
 */
void MainWindow::remove_item_from_tree(MyQTreeWidgetItem *my_item) {
    set_status_bar("Remove item");
    delete my_item;
}

/**
 * @brief MainWindow::toggle_toolbar
 * This method will toggle the toolbar depending on whether the overlay and analysis is showing or not.
 * It is switching between a toolbar that contains items as save/add/load and another that
 * contains drawing tools, and a third containing analysis tools.
 * This is invoked when the overlay and analysis tool is activated and deactivated.
 */
void MainWindow::toggle_toolbar() {
    if (mvideo_player->is_showing_analysis_tool()) {
        ui->toolBar_analysis->show();
        ui->toolBar->hide();
        ui->toolBar_overlay->hide();
    } else if (mvideo_player->is_showing_overlay()) {
        ui->toolBar_analysis->hide();
        ui->toolBar->hide();
        ui->toolBar_overlay->show();
    } else {
        ui->toolBar_analysis->hide();
        ui->toolBar->show();
        ui->toolBar_overlay->hide();
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

/**
 * @brief MainWindow::get_project_from_object
 * @param item item you want to get the topmost parent of.
 * @return
 */
QTreeWidgetItem *MainWindow::get_project_from_object(QTreeWidgetItem* item) {
    QTreeWidgetItem *project = item;
    while (project->parent()){
        project = project->parent();
    }
    return project;
}

/**
 * @brief MainWindow::on_actionShow_hide_analysis_area_triggered
 * Toggles the choosing of an analysis area.
 */
void MainWindow::on_actionShow_hide_analysis_area_triggered() {
    mvideo_player->toggle_analysis_area();
    toggle_toolbar();
    if (mvideo_player->is_showing_analysis_tool()) {
        set_status_bar("Showing analysis area tool. Select your area by clicking on the video.");
    } else {
        set_status_bar("Hiding analysis area tool.");
    }
}

/**
 * @brief MainWindow::on_actionContrast_Brightness_triggered
 * Opens a window to choose contrast and brightness in.
 */
void MainWindow::on_actionContrast_Brightness_triggered() {
    float contrast = mvideo_player->get_contrast();
    int brightness = mvideo_player->get_brightness();

    // Create the texts shown in the dialog
    std::stringstream contrast_ss;
    contrast_ss << "Contrast [" << mvideo_player->CONTRAST_MIN << " – " << mvideo_player->CONTRAST_MAX <<
                   "] (default: "<< mvideo_player->CONTRAST_DEFAULT <<"): ";
    QString contrast_text = QString::fromStdString(contrast_ss.str());
    std::stringstream brightness_ss;
    brightness_ss << "Brightness [" << mvideo_player->BRIGHTNESS_MIN << " – " << mvideo_player->BRIGHTNESS_MAX <<
                     "] (default: "<< mvideo_player->BRIGHTNESS_DEFAULT <<"): ";
    QString brightness_text = QString::fromStdString(brightness_ss.str());

    // Create the dialog
    CustomDialog dialog("Contrast & Brightness", this);
    dialog.addLabel("Enter values:");
    dialog.addDblSpinBoxF(contrast_text, (float) mvideo_player->CONTRAST_MIN, (float) mvideo_player->CONTRAST_MAX,
                          &contrast, mvideo_player->CONTRAST_DECIMALS, (float) mvideo_player->CONTRAST_STEP,
                          "Choose contrast value with the input box.");
    dialog.addSpinBox(brightness_text, mvideo_player->BRIGHTNESS_MIN, mvideo_player->BRIGHTNESS_MAX,
                      &brightness, mvideo_player->BRIGHTNESS_STEP, "Choose brightness value with the input box.");

    // Show the dialog (execution will stop here until the dialog is finished)
    dialog.exec();

    if (dialog.wasCancelled()) {
        return;
    }
    mvideo_player->set_contrast(contrast);
    mvideo_player->set_brightness(brightness);
}
