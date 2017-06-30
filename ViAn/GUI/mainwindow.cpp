#include "mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <QCloseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QTime>
#include <QDebug>
#include <chrono>
#include <thread>
#include "Video/shapes/shape.h"
#include "Analysis/MotionDetection.h"
#include "Analysis/AnalysisMethod.h"
#include "Toolbars/maintoolbar.h"
#include "Toolbars/drawingtoolbar.h"
#include "manipulatordialog.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent a QWidget variable
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    QDockWidget* project_dock = new QDockWidget(tr("Projects"), this);
    QDockWidget* bookmark_dock = new QDockWidget(tr("Bookmarks"), this);
    project_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    bookmark_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toggle_project_wgt = project_dock->toggleViewAction();
    toggle_bookmark_wgt = bookmark_dock->toggleViewAction();

    // Initialize video widget
    video_wgt = new VideoWidget();
    video_wgt->setMinimumSize(VIDEO_WGT_WIDTH * SIZE_MULTIPLIER, VIDEO_WGT_HEIGHT * SIZE_MULTIPLIER); // width and height sets aspect ratio
    setCentralWidget(video_wgt);

    // Initialize project widget
    project_wgt = new ProjectWidget();
    project_dock->setWidget(project_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, project_dock);

    // Initialize analysis widget
    analysis_wgt = new AnalysisWidget();
    connect(video_wgt, SIGNAL(start_analysis(VideoProject*)), project_wgt, SLOT(start_analysis(VideoProject*)));
    connect(project_wgt, SIGNAL(begin_analysis(std::string,std::string,QTreeWidgetItem*)), analysis_wgt, SLOT(start_analysis(std::string,std::string,QTreeWidgetItem*)));


    // Initialize bookmark widget
    bookmark_wgt = new BookmarkWidget();
    addDockWidget(Qt::RightDockWidgetArea, bookmark_dock);
    std::vector<std::string> tags = {"one", "two"};
    bookmark_wgt->add_bookmark("bookmark description", tags);
    connect(video_wgt, SIGNAL(new_bookmark(int,cv::Mat)), bookmark_wgt, SLOT(create_bookmark(int,cv::Mat)));
    bookmark_dock->setWidget(bookmark_wgt);

    //Initialize menu bar
    init_file_menu();
    init_edit_menu();
    init_view_menu();
    init_analysis_menu();
    init_tools_menu();
    init_help_menu();

    // Main toolbar
    MainToolbar* main_toolbar = new MainToolbar();
    QAction* toggle_toolbar = main_toolbar->toggleViewAction();
    addToolBar(main_toolbar);
    connect(main_toolbar->add_video_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);

    // Draw toolbar
    DrawingToolbar* draw_toolbar = new DrawingToolbar();
    QAction* toggle_draw_toolbar = draw_toolbar->toggleViewAction();
    addToolBar(draw_toolbar);
    connect(main_toolbar->toggle_draw_toolbar_act, &QAction::triggered, toggle_draw_toolbar, &QAction::trigger);   

    // Status bar
    status_bar = new StatusBar();
    setStatusBar(status_bar);
    connect(this, SIGNAL(set_status_bar(QString)), status_bar, SLOT(on_set_status_bar(QString)));
    connect(video_wgt, SIGNAL(set_status_bar(QString)), status_bar, SLOT(on_set_status_bar(QString)));
    connect(project_wgt, SIGNAL(set_status_bar(QString)), status_bar, SLOT(on_set_status_bar(QString)));
    connect(draw_toolbar, SIGNAL(set_status_bar(QString)), status_bar, SLOT(on_set_status_bar(QString)));
    connect(analysis_wgt, &AnalysisWidget::add_analysis_bar, status_bar, &StatusBar::add_analysis_bar);
    connect(analysis_wgt, &AnalysisWidget::remove_analysis_bar, status_bar, &StatusBar::remove_analysis_bar);
    connect(analysis_wgt, SIGNAL(show_progress(int)), status_bar, SLOT(update_analysis_bar(int)));

    connect(project_wgt, &ProjectWidget::marked_video, video_wgt, &VideoWidget::load_marked_video);
    connect(analysis_wgt, SIGNAL(name_in_tree(QTreeWidgetItem*,QString)), project_wgt, SLOT(set_tree_item_name(QTreeWidgetItem*,QString)));

    connect(project_wgt, SIGNAL(marked_analysis(Analysis*)), video_wgt->frame_wgt, SLOT(set_analysis(Analysis*)));
    connect(project_wgt, SIGNAL(set_detections(bool)), video_wgt->frame_wgt, SLOT(set_detections(bool)));
    connect(project_wgt, SIGNAL(enable_poi_btns(bool)), video_wgt, SLOT(enable_poi_btns(bool)));
}


/**
 * @brief MainWindow::~MainWindow
 * Destructor
 */
MainWindow::~MainWindow() {
}

/**
 * @brief MainWindow::init_file_menu
 * Set up the file menu
 */
void MainWindow::init_file_menu() {
    QMenu* file_menu = menuBar()->addMenu(tr("&File"));

    // Init actions
    QAction* new_project_act = new QAction(tr("&New project"), this);
    QAction* add_vid_act = new QAction(tr("&Add video"), this);
    QAction* open_project_act = new QAction(tr("&Open project"), this);
    QAction* save_project_act = new QAction(tr("&Save project"), this);
    QAction* gen_report_act = new QAction(tr("&Generate report"), this);
    QAction* close_project_act = new QAction(tr("&Close project"), this);
    QAction* remove_project_act = new QAction(tr("&Remove project"), this);
    QAction* quit_act = new QAction(tr("&Quit"), this);

    // Set icons
    //new_project_act->setIcon(QIcon("../ViAn/Icons/....png"));     //add if wanted
    add_vid_act->setIcon(QIcon("../ViAn/Icons/add_video.png"));
    open_project_act->setIcon(QIcon("../ViAn/Icons/open.png"));
    save_project_act->setIcon(QIcon("../ViAn/Icons/save.png"));
    //gen_report_act->setIcon(QIcon("../ViAn/Icons/....png"));      //add if wanted
    //close_project_act->setIcon(QIcon("../ViAn/Icons/....png"));   //add if wanted
    remove_project_act->setIcon(QIcon("../ViAn/Icons/clear.png"));
    quit_act->setIcon(QIcon("../ViAn/Icons/quit.png"));

    // Add actions to the menu
    file_menu->addAction(new_project_act);
    file_menu->addAction(add_vid_act);
    file_menu->addAction(open_project_act);
    file_menu->addAction(save_project_act);
    file_menu->addAction(close_project_act);
    file_menu->addAction(remove_project_act);
    file_menu->addSeparator();
    file_menu->addAction(gen_report_act);
    file_menu->addSeparator();
    file_menu->addAction(quit_act);

    // Set shortcuts
    new_project_act->setShortcuts(QKeySequence::New);       //Ctrl + N
    add_vid_act->setShortcuts(QKeySequence::SelectAll);     //Ctrl + A
    open_project_act->setShortcuts(QKeySequence::Open);     //Ctrl + O
    save_project_act->setShortcuts(QKeySequence::Save);     //Ctrl + S
    close_project_act->setShortcuts(QKeySequence::Close);   //Ctrl + F4
    remove_project_act->setShortcuts(QKeySequence::Delete); //Del
    gen_report_act->setShortcuts(QKeySequence::Print);      //Ctrl + P
    quit_act->setShortcut(QKeySequence(tr("Ctrl+E")));      //Ctrl + E

    // Set shortcuts
    new_project_act->setStatusTip(tr("Create a new project"));
    add_vid_act->setStatusTip(tr("Add video"));
    open_project_act->setStatusTip(tr("Load project"));
    save_project_act->setStatusTip(tr("Save project"));
    close_project_act->setStatusTip(tr("Close project"));
    remove_project_act->setStatusTip(tr("Remove project"));
    gen_report_act->setStatusTip(tr("Generate report"));
    quit_act->setStatusTip(tr("Quit the application"));

    // Connet with signals and slots
    connect(new_project_act, &QAction::triggered, project_wgt, &ProjectWidget::new_project);
    connect(add_vid_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);
    connect(open_project_act, &QAction::triggered, project_wgt, &ProjectWidget::open_project);
    connect(save_project_act, &QAction::triggered, project_wgt, &ProjectWidget::save_project);
    connect(gen_report_act, &QAction::triggered, this, &MainWindow::gen_report);
    connect(close_project_act, &QAction::triggered, project_wgt, &ProjectWidget::close_project);
    connect(remove_project_act, &QAction::triggered, project_wgt, &ProjectWidget::remove_project);
    connect(quit_act, &QAction::triggered, this, &QWidget::close);
}

/**
 * @brief MainWindow::init_edit_menu
 * Set up the edit menu
 */
void MainWindow::init_edit_menu() {
    QMenu* edit_menu = menuBar()->addMenu(tr("&Edit"));

    QAction* cont_bri_act = new QAction(tr("&Contrast/Brightness"), this);
    QAction* cw_act = new QAction(tr("&Rotate 90°"), this);
    QAction* ccw_act = new QAction(tr("Ro&tate 90°"), this);
    QAction* options_act = new QAction(tr("&Options"), this);

    cont_bri_act->setIcon(QIcon("../ViAn/Icons/screen.png"));
    cw_act->setIcon(QIcon("../ViAn/Icons/right.png"));
    ccw_act->setIcon(QIcon("../ViAn/Icons/left.png"));
    options_act->setIcon(QIcon("../ViAn/Icons/gears.png"));

    edit_menu->addAction(cont_bri_act);
    edit_menu->addAction(cw_act);
    edit_menu->addAction(ccw_act);
    edit_menu->addSeparator();
    edit_menu->addAction(options_act);

    cont_bri_act->setStatusTip(tr("Contrast and brightness"));
    cw_act->setStatusTip(tr("Rotate clockwise"));
    ccw_act->setStatusTip(tr("Rotate counter clockwise"));
    options_act->setStatusTip(tr("Program options"));

    connect(cont_bri_act, &QAction::triggered, this, &MainWindow::cont_bri);
    connect(cw_act, &QAction::triggered, video_wgt->m_video_player, &video_player::rotate_right);
    connect(ccw_act, &QAction::triggered, video_wgt->m_video_player, &video_player::rotate_left);
    connect(options_act, &QAction::triggered, this, &MainWindow::options);
}

/**
 * @brief MainWindow::init_view_menu
 * Set up the view menu
 */
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

    toggle_project_wgt->setStatusTip(tr("Show/hide project widget"));
    toggle_bookmark_wgt->setStatusTip(tr("Show/hide bookmark widget"));
    annotation_act->setStatusTip(tr("Toggle annotations on/off"));
    detection_act->setStatusTip(tr("Toggle detections on/off"));
}

/**
 * @brief MainWindow::init_analysis_menu
 * Set up the analysis menu
 */
void MainWindow::init_analysis_menu() {
    QMenu* analysis_menu = menuBar()->addMenu(tr("&Analysis"));

    QAction* analysis_act = new QAction(tr("&Perform_analysis"), this);
    analysis_act->setIcon(QIcon("../ViAn/Icons/analysis.png"));
    analysis_act->setStatusTip(tr("Perform analysis"));
    analysis_menu->addAction(analysis_act);
    connect(analysis_act, &QAction::triggered, video_wgt, &VideoWidget::analysis_btn_clicked);
}

/**
 * @brief MainWindow::init_tools_menu
 * Set up the tools menu
 */
void MainWindow::init_tools_menu() {
    QMenu* tool_menu = menuBar()->addMenu(tr("&Tools"));

    QAction* color_act = new QAction(tr("&Color"), this);
    QAction* undo_act = new QAction(tr("&Undo"), this);
    QAction* clear_act = new QAction(tr("C&lear"), this);
    QAction* zoom_in_act = new QAction(tr("Zoom &in"), this);
    QAction* zoom_out_act = new QAction(tr("Zoom &out"), this);
    QAction* fit_screen_act = new QAction(tr("&Fit to screen"), this);
    QAction* move_act = new QAction(tr("&Panning tool"), this);
    QAction* rectangle_act = new QAction(tr("&Rectangle"), this);
    QAction* circle_act = new QAction(tr("&Circle"), this);
    QAction* line_act = new QAction(tr("&Line"), this);
    QAction* arrow_act = new QAction(tr("&Arrow"), this);
    QAction* pen_act = new QAction(tr("&Pen"), this);
    QAction* text_act = new QAction(tr("&Text"), this);

    color_act->setIcon(QIcon("../ViAn/Icons/color.png"));
    undo_act->setIcon(QIcon("../ViAn/Icons/undo.png"));
    clear_act->setIcon(QIcon("../ViAn/Icons/clear.png"));
    zoom_in_act->setIcon(QIcon("../ViAn/Icons/zoom_in.png"));
    zoom_out_act->setIcon(QIcon("../ViAn/Icons/zoom_out.png"));
    fit_screen_act->setIcon(QIcon("../ViAn/Icons/fit_screen.png"));
    move_act->setIcon(QIcon("../ViAn/Icons/move.png"));
    rectangle_act->setIcon(QIcon("../ViAn/Icons/box.png"));
    circle_act->setIcon(QIcon("../ViAn/Icons/circle.png"));
    line_act->setIcon(QIcon("../ViAn/Icons/line.png"));
    arrow_act->setIcon(QIcon("../ViAn/Icons/arrow.png"));
    pen_act->setIcon(QIcon("../ViAn/Icons/pen.png"));
    text_act->setIcon(QIcon("../ViAn/Icons/text.png"));

    tool_menu->addAction(color_act);
    QMenu* drawing_tools = tool_menu->addMenu(tr("&Shapes"));
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

    undo_act->setShortcut(QKeySequence::Undo);
    fit_screen_act->setShortcut(tr("Ctrl+F"));

    color_act->setStatusTip(tr("Color picker"));
    undo_act->setStatusTip(tr("Undo last drawing"));
    clear_act->setStatusTip(tr("Clear all drawings"));
    zoom_in_act->setStatusTip(tr("Zoom in"));
    zoom_out_act->setStatusTip(tr("Zoom out"));
    fit_screen_act->setStatusTip(tr("Fit to screen"));
    move_act->setStatusTip(tr("Panning tool"));
    rectangle_act->setStatusTip(tr("Rectangle tool"));
    circle_act->setStatusTip(tr("Circle tool"));
    line_act->setStatusTip(tr("Line tool"));
    arrow_act->setStatusTip(tr("Arrow tool"));
    pen_act->setStatusTip(tr("Pen tool"));
    text_act->setStatusTip(tr("Text tool"));

    //Connect
}

/**
 * @brief MainWindow::init_help_menu
 * Set up the help menu
 */
void MainWindow::init_help_menu() {
    QMenu* help_menu = menuBar()->addMenu(tr("&Help"));
    QAction* help_act = new QAction(tr("Open manual"), this);
    help_act->setIcon(QIcon("../ViAn/Icons/question.png"));
    help_menu->addAction(help_act);
    help_act->setShortcut(tr("Ctrl+h"));
    help_act->setStatusTip(tr("Help"));

    //connect
}

/**
 * @brief MainWindow::gen_report
 * runs when the generate report action is triggered
 */
void MainWindow::gen_report() {
    emit set_status_bar("Generating report. Please wait.");
}

/**
 * @brief MainWindow::cont_bri
 * Contrast/brightness triggered.
 * Creates a dialog for choosing contrast and brightness values.
 */
void MainWindow::cont_bri() {
    emit set_status_bar("Opening contrast/brightness settings");
    ManipulatorDialog* man_dialog = new ManipulatorDialog(this);
    connect(man_dialog, SIGNAL(values(int,double)), video_wgt->m_video_player, SLOT(set_bright_cont(int,double)));
    man_dialog->exec();
}

/**
 * @brief MainWindow::options
 *  runs when the options action is triggered
 */
void MainWindow::options() {
    emit set_status_bar("Opening options.");
}
