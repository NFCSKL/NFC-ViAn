#include "mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <QCloseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QProgressDialog>
#include <chrono>
#include <thread>
#include "Video/shapes/shape.h"
#include "Analysis/motiondetection.h"
#include "Analysis/analysismethod.h"
#include "Toolbars/maintoolbar.h"
#include "Toolbars/drawingtoolbar.h"
#include "manipulatordialog.h"
#include "GUI/frameexporterdialog.h"


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
    project_wgt = new ProjectWidget(); //mianwindow->eemit from frame_wgt to videoplayer/overlay
    project_dock->setWidget(project_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, project_dock);

    // Initialize analysis widget
    analysis_wgt = new AnalysisWidget();

    connect(video_wgt, SIGNAL(start_analysis(VideoProject*, AnalysisSettings*)), project_wgt, SLOT(start_analysis(VideoProject*, AnalysisSettings*)));
    connect(video_wgt->frame_wgt, SIGNAL(quick_analysis(AnalysisSettings*)), video_wgt, SLOT(quick_analysis(AnalysisSettings*)));
    connect(project_wgt, SIGNAL(begin_analysis(QTreeWidgetItem*, AnalysisMethod*)),
            analysis_wgt, SLOT(start_analysis(QTreeWidgetItem*, AnalysisMethod*)));

    // Initialize bookmark widget
    bookmark_wgt = new BookmarkWidget();
    bookmark_wgt->setWindowFlags(Qt::Window);
    addDockWidget(Qt::RightDockWidgetArea, bookmark_dock);
    bookmark_dock->close();

    connect(video_wgt, SIGNAL(new_bookmark(VideoProject*,int,cv::Mat)), bookmark_wgt, SLOT(create_bookmark(VideoProject*,int,cv::Mat)));
    connect(project_wgt, SIGNAL(proj_path(std::string)), bookmark_wgt, SLOT(set_path(std::string)));
    connect(project_wgt, SIGNAL(load_bookmarks(VideoProject*)), bookmark_wgt, SLOT(load_bookmarks(VideoProject*)));
    connect(bookmark_wgt, SIGNAL(play_bookmark_video(VideoProject*,int)), video_wgt, SLOT(load_marked_video(VideoProject*)));
    connect(project_wgt, &ProjectWidget::project_closed, bookmark_wgt, &BookmarkWidget::clear_bookmarks);
    bookmark_dock->setWidget(bookmark_wgt);    

    //Initialize menu bar
    init_file_menu();
    init_edit_menu();
    init_view_menu();
    init_analysis_menu();
    init_interval_menu();
    init_tools_menu();
    init_help_menu();

    // Main toolbar
    MainToolbar* main_toolbar = new MainToolbar();
    main_toolbar->setWindowTitle(tr("Main toolbar"));
    addToolBar(main_toolbar);
    connect(main_toolbar->add_video_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);
    connect(main_toolbar->save_act, &QAction::triggered, project_wgt, &ProjectWidget::save_project);
    connect(main_toolbar->open_act, &QAction::triggered, this, &MainWindow::open_project_dialog);

    // Draw toolbar
    DrawingToolbar* draw_toolbar = new DrawingToolbar();
    draw_toolbar->setWindowTitle(tr("Draw toolbar"));
    QAction* toggle_draw_toolbar = draw_toolbar->toggleViewAction();
    addToolBar(draw_toolbar);
    connect(main_toolbar->toggle_draw_toolbar_act, &QAction::triggered, toggle_draw_toolbar, &QAction::trigger);   
    connect(draw_toolbar, SIGNAL(set_color(QColor)), video_wgt->frame_wgt, SLOT(set_overlay_color(QColor)));
    connect(draw_toolbar, SIGNAL(set_overlay_tool(SHAPES)), video_wgt->frame_wgt, SLOT(set_tool(SHAPES)));
    connect(draw_toolbar->undo_tool_act, &QAction::triggered, this, &MainWindow::undo);
    connect(draw_toolbar->redo_tool_act, &QAction::triggered, this, &MainWindow::redo);
    connect(draw_toolbar->clear_tool_act, &QAction::triggered, this, &MainWindow::clear);
    connect(color_act, &QAction::triggered, draw_toolbar, &DrawingToolbar::color_tool_clicked);

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

    connect(project_wgt, &ProjectWidget::marked_video, video_wgt->frame_wgt, &FrameWidget::clear_analysis);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt->playback_slider, &AnalysisSlider::clear_slider);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt, &VideoWidget::load_marked_video);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt, &VideoWidget::clear_tag);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt->frame_wgt, &FrameWidget::set_video_project);

    connect(analysis_wgt, SIGNAL(name_in_tree(QTreeWidgetItem*,QString)), project_wgt, SLOT(set_tree_item_name(QTreeWidgetItem*,QString)));

    connect(project_wgt, SIGNAL(marked_analysis(AnalysisProxy*)), video_wgt->frame_wgt, SLOT(set_analysis(AnalysisProxy*)));
    connect(project_wgt, SIGNAL(marked_basic_analysis(BasicAnalysis*)), video_wgt->playback_slider, SLOT(set_basic_analysis(BasicAnalysis*)));

    connect(project_wgt, SIGNAL(set_detections(bool)), video_wgt->frame_wgt, SLOT(set_detections(bool)));

    connect(project_wgt, SIGNAL(enable_poi_btns(bool,bool)), video_wgt, SLOT(enable_poi_btns(bool,bool)));
    connect(project_wgt, SIGNAL(enable_tag_btn(bool)), video_wgt, SLOT(enable_tag_btn(bool)));

    connect(project_wgt, SIGNAL(set_poi_slider(bool)), video_wgt->playback_slider, SLOT(set_show_pois(bool)));
    connect(project_wgt, SIGNAL(set_tag_slider(bool)), video_wgt->playback_slider, SLOT(set_show_tags(bool)));

    connect(project_wgt, SIGNAL(marked_basic_analysis(BasicAnalysis*)), video_wgt, SLOT(set_basic_analysis(BasicAnalysis*)));

    connect(video_wgt, SIGNAL(add_basic_analysis(VideoProject*, BasicAnalysis*)), project_wgt, SLOT(add_basic_analysis(VideoProject*, BasicAnalysis*)));

    connect(project_wgt, &ProjectWidget::remove_overlay, video_wgt, &VideoWidget::set_overlay_removed);

    connect(project_wgt, &ProjectWidget::update_frame, video_wgt->playback_slider, &AnalysisSlider::update);
    connect(project_wgt, &ProjectWidget::update_frame, video_wgt->frame_wgt, &FrameWidget::update);
    connect(this, &MainWindow::open_project, project_wgt, &ProjectWidget::open_project);

    // Recent projects menu
    RecentProjectDialog* rp_dialog = new RecentProjectDialog(this);
    connect(rp_dialog, &RecentProjectDialog::open_project, project_wgt, &ProjectWidget::open_project);
    connect(rp_dialog, &RecentProjectDialog::new_project, project_wgt, &ProjectWidget::new_project);
    connect(rp_dialog, &RecentProjectDialog::open_project_from_file, this, &MainWindow::open_project_dialog);
    QTimer::singleShot(0, rp_dialog, SLOT(exec()));
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
    connect(open_project_act, &QAction::triggered, this, &MainWindow::open_project_dialog);
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
    connect(options_act, &QAction::triggered, this, &MainWindow::options);
    connect(cw_act, &QAction::triggered, video_wgt, &VideoWidget::rotate_cw);
    connect(ccw_act, &QAction::triggered, video_wgt, &VideoWidget::rotate_ccw);
}

/**
 * @brief MainWindow::init_view_menu
 * Set up the view menu
 */
void MainWindow::init_view_menu() {
    QMenu* view_menu = menuBar()->addMenu(tr("&View"));

    detect_intv_act = new QAction(tr("&Detection intervals"), this);      //Slider pois
    bound_box_act = new QAction(tr("&Bounding boxes"), this);        //Video oois
    interval_act = new QAction(tr("&Interval"), this);
    drawing_act = new QAction(tr("&Paintings"), this);

    detect_intv_act->setCheckable(true);
    bound_box_act->setCheckable(true);
    interval_act->setCheckable(true);
    drawing_act->setCheckable(true);

    detect_intv_act->setChecked(true);
    bound_box_act->setChecked(true);
    interval_act->setChecked(true);
    drawing_act->setChecked(true);

    view_menu->addAction(toggle_project_wgt);
    view_menu->addAction(toggle_bookmark_wgt);
    view_menu->addSeparator();
    view_menu->addAction(detect_intv_act);
    view_menu->addAction(bound_box_act);
    view_menu->addAction(interval_act);
    view_menu->addAction(drawing_act);

    toggle_project_wgt->setStatusTip(tr("Show/hide project widget"));
    toggle_bookmark_wgt->setStatusTip(tr("Show/hide bookmark widget"));
    detect_intv_act->setStatusTip(tr("Toggle annotations on/off"));
    bound_box_act->setStatusTip(tr("Toggle detections on/off"));
    interval_act->setStatusTip(tr("Toggle interval on/off"));
    drawing_act->setStatusTip(tr("Toggle drawings on/off"));

    connect(bound_box_act, &QAction::toggled, video_wgt->frame_wgt, &FrameWidget::set_show_detections);
    connect(bound_box_act, &QAction::toggled, video_wgt->frame_wgt, &FrameWidget::update);
    connect(detect_intv_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::set_show_on_slider);
    connect(detect_intv_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::update);
    connect(interval_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::set_show_interval);
    connect(interval_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::update);
    //connect(drawing_act, &QAction::toggled, video_wgt->frame_wgt->get_overlay(), &Overlay::set_showing_overlay);
}

/**
 * @brief MainWindow::init_analysis_menu
 * Set up the analysis menu
 */
void MainWindow::init_analysis_menu() {
    QMenu* analysis_menu = menuBar()->addMenu(tr("&Analysis"));

    QAction* analysis_act = new QAction(tr("&Perform analysis"), this);
    analysis_act->setIcon(QIcon("../ViAn/Icons/analysis.png"));
    analysis_act->setStatusTip(tr("Perform analysis"));
    analysis_menu->addAction(analysis_act);
    connect(analysis_act, &QAction::triggered, project_wgt, &ProjectWidget::advanced_analysis);
}

void MainWindow::init_interval_menu() {
    QMenu* interval_menu = menuBar()->addMenu(tr("&Interval"));

    QAction* tag_interval_act = new QAction(tr("&Tag interval"), this);
    QAction* rm_tag_interval_act = new QAction(tr("&Remove tag on interval"), this);
    QAction* rm_interval_act = new QAction(tr("&Delete interval"), this);
    tag_interval_act->setShortcut(tr("Shift+T"));
    rm_tag_interval_act->setShortcut(tr("Shift+R"));
    interval_menu->addAction(tag_interval_act);
    interval_menu->addAction(rm_tag_interval_act);
    interval_menu->addAction(rm_interval_act);
    connect(tag_interval_act, &QAction::triggered, video_wgt, &VideoWidget::tag_interval);
    connect(rm_tag_interval_act, &QAction::triggered, video_wgt, &VideoWidget::remove_tag_interval);
    connect(rm_interval_act, &QAction::triggered, video_wgt, &VideoWidget::delete_interval);
}

/**
 * @brief MainWindow::init_tools_menu
 * Set up the tools menu
 */
void MainWindow::init_tools_menu() {
    QMenu* tool_menu = menuBar()->addMenu(tr("&Tools"));

    color_act = new QAction(tr("&Color"), this);
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
    QAction* undo_act = new QAction(tr("&Undo"), this);
    QAction* redo_act = new QAction(tr("Re&do"), this);
    QAction* clear_act = new QAction(tr("C&lear"), this);

    QAction* export_act  =new QAction(tr("&Frames"), this);

    color_act->setIcon(QIcon("../ViAn/Icons/color.png"));
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
    undo_act->setIcon(QIcon("../ViAn/Icons/undo.png"));
    redo_act->setIcon(QIcon("../ViAn/Icons/redo.png"));
    clear_act->setIcon(QIcon("../ViAn/Icons/clear.png"));

    // Export submenu
    QMenu* export_menu = tool_menu->addMenu(tr("&Export"));
    export_menu->addAction(export_act);

    tool_menu->addAction(color_act);
    QMenu* drawing_tools = tool_menu->addMenu(tr("&Shapes"));
    drawing_tools->addAction(rectangle_act);
    drawing_tools->addAction(circle_act);
    drawing_tools->addAction(line_act);
    drawing_tools->addAction(arrow_act);
    drawing_tools->addAction(pen_act);
    drawing_tools->addAction(text_act);
    tool_menu->addAction(undo_act);
    tool_menu->addAction(redo_act);
    tool_menu->addAction(clear_act);
    tool_menu->addSeparator();
    tool_menu->addAction(zoom_in_act);
    tool_menu->addAction(zoom_out_act);
    tool_menu->addAction(fit_screen_act);
    tool_menu->addAction(move_act);

    undo_act->setShortcut(QKeySequence::Undo);
    redo_act->setShortcut(QKeySequence::Redo);
    clear_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
    fit_screen_act->setShortcut(tr("Ctrl+F"));

    color_act->setStatusTip(tr("Color picker"));
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
    undo_act->setStatusTip(tr("Undo last drawing"));
    redo_act->setStatusTip(tr("Redo last drawing"));
    clear_act->setStatusTip(tr("Clear all drawings"));

    //Connect
    connect(export_act, &QAction::triggered, this, &MainWindow::export_images);
    connect(rectangle_act, &QAction::triggered, this, &MainWindow::rectangle);
    connect(circle_act, &QAction::triggered, this, &MainWindow::circle);
    connect(line_act, &QAction::triggered, this, &MainWindow::line);
    connect(arrow_act, &QAction::triggered, this, &MainWindow::arrow);
    connect(pen_act, &QAction::triggered, this, &MainWindow::pen);
    connect(text_act, &QAction::triggered, this, &MainWindow::text);
    connect(undo_act, &QAction::triggered, this, &MainWindow::undo);
    connect(redo_act, &QAction::triggered, this, &MainWindow::redo);
    connect(clear_act, &QAction::triggered, this, &MainWindow::clear);
    connect(zoom_in_act, &QAction::triggered, this, &MainWindow::zoom);
    connect(move_act, &QAction::triggered, this, &MainWindow::move);
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

void MainWindow::rectangle() {
    video_wgt->frame_wgt->set_tool(RECTANGLE);
}

void MainWindow::circle() {
    video_wgt->frame_wgt->set_tool(CIRCLE);
}

void MainWindow::line() {
    video_wgt->frame_wgt->set_tool(LINE);
}

void MainWindow::arrow() {
    video_wgt->frame_wgt->set_tool(ARROW);
}

void MainWindow::pen() {
    video_wgt->frame_wgt->set_tool(PEN);
}

void MainWindow::text() {
    video_wgt->frame_wgt->set_tool(TEXT);
}

void MainWindow::undo() {
    video_wgt->set_undo();
}

void MainWindow::redo() {
    video_wgt->set_redo();
}

void MainWindow::clear() {
    video_wgt->set_clear_drawings();
}

void MainWindow::zoom() {
    video_wgt->frame_wgt->set_tool(ZOOM);
}

void MainWindow::move() {
    video_wgt->frame_wgt->set_tool(MOVE);
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
    connect(man_dialog, SIGNAL(values(int,double)), video_wgt, SLOT(update_brightness_contrast(int,double)));
    man_dialog->exec();
}

void MainWindow::export_images(){
    std::pair<int, int> interval = video_wgt->get_frame_interval();
    VideoProject* vid_proj = video_wgt->get_current_video_project();
    if (vid_proj == nullptr){
        set_status_bar("A video needs to be selected");
        return;
    }

    if (interval.first > interval.second) {
        int tmp = interval.second;
        interval.second = interval.first;
        interval.first = tmp;
    }
    ImageExporter* im_exp = new ImageExporter();
    FrameExporterDialog exporter_dialog(im_exp, vid_proj->get_video(), project_wgt->m_proj->getDir(),
                                        video_wgt->get_current_video_length() - 1,
                                        interval);
    if (!exporter_dialog.exec()){
        delete im_exp;
        return;
    }

    interval = im_exp->get_interval();
    QProgressDialog* progress = new QProgressDialog(
                "Exporting images...", "Abort", 0, abs(interval.first - interval.second) + 1, this, Qt::WindowMinimizeButtonHint);

    connect(im_exp, &ImageExporter::finished_msg, this, &MainWindow::set_status_bar);
    connect(progress, &QProgressDialog::canceled, im_exp, &ImageExporter::abort);
    connect (im_exp, &ImageExporter::update_progress, progress, &QProgressDialog::setValue);

    QThread* exporter_thread = new QThread;
    im_exp->moveToThread(exporter_thread);
    connect(exporter_thread, &QThread::started, im_exp, &ImageExporter::export_images);
    connect(im_exp, &ImageExporter::finished, exporter_thread, &QThread::quit);
    connect(im_exp, &ImageExporter::finished, im_exp, &ImageExporter::deleteLater);
    connect(exporter_thread, &QThread::finished, exporter_thread, &QThread::deleteLater);
    progress->show();
    exporter_thread->start();
}

/**
 * @brief MainWindow::options
 *  runs when the options action is triggered
 */
void MainWindow::options() {
    emit set_status_bar("Opening options");
}

void MainWindow::open_project_dialog(){
    QString project_path = QFileDialog().getOpenFileName(this, tr("Open project"), QDir::homePath());
    open_project(project_path);
}
