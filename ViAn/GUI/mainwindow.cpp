#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <QCloseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QTime>
#include <chrono>
#include <thread>
#include "icononbuttonhandler.h"
#include "Video/shapes/shape.h"
#include "Analysis/MotionDetection.h"
#include "Analysis/AnalysisMethod.h"
#include "Toolbars/maintoolbar.h"
#include "Toolbars/drawingtoolbar.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent a QWidget variable
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    QMenuBar* menuBar = new QMenuBar();
    QDockWidget* project_dock = new QDockWidget(tr("Projects"), this);
    QDockWidget* bookmark_dock = new QDockWidget(tr("Bookmarks"), this);
    project_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    bookmark_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toggle_project_wgt = project_dock->toggleViewAction();
    toggle_bookmark_wgt = bookmark_dock->toggleViewAction();

    // Initialize Video Widget
    video_wgt = new VideoWidget();
    video_wgt->setMinimumSize(600,400); //TODO fix magic
    setCentralWidget(video_wgt);

    // Initialize project widget
    project_wgt = new ProjectWidget();
    project_dock->setWidget(project_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, project_dock);


    // Initialize bookmark widget
    bookmark_wgt = new BookmarkWidget();
    bookmark_dock->setWidget(bookmark_wgt);
    addDockWidget(Qt::RightDockWidgetArea, bookmark_dock);
    std::vector<std::string> tags = {"one", "two"};
    bookmark_wgt->add_bookmark("bookmark description", tags);

    //Initialize menu bar
    init_file_menu();
    init_edit_menu();
    init_view_menu();
    init_tools_menu();
    init_help_menu();

    // Main toolbar
    MainToolbar* main_toolbar = new MainToolbar();
    QAction* toogle_toolbar = main_toolbar->toggleViewAction();
    addToolBar(main_toolbar);
    connect(main_toolbar->add_video_act, QAction::triggered, project_wgt, &ProjectWidget::add_video);

    // Draw toolbar
    DrawingToolbar* draw_toolbar = new DrawingToolbar();
    QAction* toggle_draw_toolbar = draw_toolbar->toggleViewAction();
    addToolBar(draw_toolbar);
    connect(main_toolbar->toggle_draw_toolbar_act, &QAction::triggered, toggle_draw_toolbar, &QAction::trigger);   
}


/**
 * @brief MainWindow::~MainWindow
 * Destructor
 */
MainWindow::~MainWindow() {
}

void MainWindow::init_file_menu() {
    QMenu* file_menu = menuBar()->addMenu(tr("File"));
    QAction* new_project_act = new QAction(tr("New project"), this);
    QAction* open_project_act = new QAction(tr("Open project"), this);
    QAction* save_project_act = new QAction(tr("Save project"), this);
    QAction* gen_report_act = new QAction(tr("Generate report"), this);
    QAction* close_project_act = new QAction(tr("Close project"), this);
    QAction* remove_project_act = new QAction(tr("Remove project"), this);
    QAction* add_vid_act = new QAction(tr("Add video"), this);

    file_menu->addAction(new_project_act);
    QMenu* add_menu = file_menu->addMenu(tr("Add"));
    add_menu->addAction(add_vid_act);
    file_menu->addAction(open_project_act);
    file_menu->addAction(save_project_act);
    file_menu->addAction(close_project_act);
    file_menu->addAction(remove_project_act);
    file_menu->addAction(gen_report_act);
    file_menu->addSeparator();
    QAction* quit_act = file_menu->addAction(tr("&Quit"), this, &QWidget::close);

    new_project_act->setShortcuts(QKeySequence::New);
    open_project_act->setShortcuts(QKeySequence::Open);
    save_project_act->setShortcuts(QKeySequence::Save);
    //gen_report_act->setShortcuts(QKeySequence::Save); TODO ADD
    close_project_act->setShortcuts(QKeySequence::Close);
    quit_act->setShortcuts(QKeySequence::Quit);

    new_project_act->setStatusTip(tr("Create a new project"));
    open_project_act->setStatusTip(tr("Load project"));
    save_project_act->setStatusTip(tr("Save project"));
    gen_report_act->setStatusTip(tr("Generate report"));
    close_project_act->setStatusTip(tr("Close project"));
    remove_project_act->setStatusTip(tr("Remove project"));
    quit_act->setStatusTip(tr("Quit the application"));

    connect(new_project_act, &QAction::triggered, project_wgt, &ProjectWidget::new_project);
    connect(add_vid_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);
    //connect(open_project_act, &QAction::triggered, this, &MainWindow::open_project);
    connect(save_project_act, &QAction::triggered, this, &MainWindow::save_project);
    connect(gen_report_act, &QAction::triggered, this, &MainWindow::gen_report);
    connect(close_project_act, &QAction::triggered, this, &MainWindow::close_project);
}

void MainWindow::init_edit_menu() {
    QMenu* edit_menu = menuBar()->addMenu(tr("Edit"));
    QAction* options_act = new QAction(tr("Options"), this);
    QAction* cont_bri_act = new QAction(tr("Contrast/Brightness"), this);
    QAction* cw_act = new QAction(tr("Rotate 90 CW"), this);
    QAction* ccw_act = new QAction(tr("Rotate 90 CCW"), this);

    edit_menu->addAction(cont_bri_act);
    edit_menu->addAction(cw_act);
    edit_menu->addAction(ccw_act);
    edit_menu->addSeparator();
    edit_menu->addAction(options_act);

    //options_act->setShortcuts(QKeySequence::);
    options_act->setStatusTip(tr("Program options"));
    connect(options_act, &QAction::triggered, this, &MainWindow::options);
}

void MainWindow::init_view_menu() {
    QMenu* view_menu = menuBar()->addMenu(tr("&View"));
    QAction* annotation_act = new QAction(tr("Annotations"), this);
    QAction* detection_act = new QAction(tr("Detections"), this);

    annotation_act->setCheckable(true);
    detection_act->setCheckable(true);

    view_menu->addAction(toggle_project_wgt);
    view_menu->addAction(toggle_bookmark_wgt);
    view_menu->addSeparator();
    view_menu->addAction(annotation_act);
    view_menu->addAction(detection_act);
}

void MainWindow::init_tools_menu() {
    QMenu* tool_menu = menuBar()->addMenu(tr("&Tools"));
    QAction* color_act = new QAction(tr("Color"), this);
    QAction* zoom_in_act = new QAction(tr("Zoom in"), this);
    QAction* zoom_out_act = new QAction(tr("Zoom out"), this);
    QAction* fit_screen_act = new QAction(tr("Fit to screen"), this);
    QAction* move_act = new QAction(tr("Panning tool"), this);
    QAction* rectangle_act = new QAction(tr("Rectangle"), this);
    QAction* circle_act = new QAction(tr("Circle"), this);
    QAction* line_act = new QAction(tr("line"), this);
    QAction* arrow_act = new QAction(tr("Arrow"), this);
    QAction* pen_act = new QAction(tr("Pen"), this);
    QAction* text_act = new QAction(tr("Text"), this);
    QAction* undo_act = new QAction(tr("Undo"), this);
    QAction* clear_act = new QAction(tr("Clear"), this);


    tool_menu->addAction(color_act);
    QMenu* drawing_tools = tool_menu->addMenu(tr("Shapes"));
    drawing_tools->addAction(rectangle_act);
    drawing_tools->addAction(circle_act);
    drawing_tools->addAction(line_act);
    drawing_tools->addAction(arrow_act);
    drawing_tools->addAction(pen_act);
    drawing_tools->addAction(text_act);
    tool_menu->addAction(undo_act);
    tool_menu->addAction(clear_act);
    tool_menu->addSeparator();
    tool_menu->addAction(zoom_in_act);
    tool_menu->addAction(zoom_out_act);
    tool_menu->addAction(fit_screen_act);
    tool_menu->addAction(move_act);
}

void MainWindow::init_help_menu() {
    QMenu* help_menu = menuBar()->addMenu(tr("&Help"));
    QAction* help_act = new QAction(tr("Open manual"), this);
    help_menu->addAction(help_act);
}


/**
 * @brief MainWindow::save_project_act
 * runs when the save project action is triggered
 */
void MainWindow::save_project() {

}

/**
 * @brief MainWindow::gen_report
 * runs when the generate report action is triggered
 */
void MainWindow::gen_report() {

}

/**
 * @brief MainWindow::on_save_project_act
 * runs when the close project action is triggered
 */
void MainWindow::close_project() {

}

/**
 * @brief MainWindow::options
 *  runs when the options action is triggered
 */
void MainWindow::options() {

}
