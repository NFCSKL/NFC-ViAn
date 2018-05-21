#include "mainwindow.h"
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
#include <QTemporaryDir>
#include <chrono>
#include <thread>
#include "Video/shapes/shapes.h"
#include "Analysis/motiondetection.h"
#include "Analysis/analysismethod.h"
#include "manipulatordialog.h"
#include "GUI/frameexporterdialog.h"


/**
 * @brief MainWindow::MainWindow
 * @param parent a QWidget variable
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    QDockWidget* project_dock = new QDockWidget(tr("Projects"), this);
    QDockWidget* bookmark_dock = new QDockWidget(tr("Bookmarks"), this);
    QDockWidget* drawing_dock = new QDockWidget(tr("Drawings"), this);
    queue_dock = new QDockWidget(tr("Analysis queue"), this);
    ana_settings_dock = new QDockWidget(tr("Analysis settings"), this);
    project_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    bookmark_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    drawing_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    queue_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    ana_settings_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toggle_project_wgt = project_dock->toggleViewAction();
    toggle_bookmark_wgt = bookmark_dock->toggleViewAction();
    toggle_drawing_wgt = drawing_dock->toggleViewAction();
    toggle_queue_wgt = queue_dock->toggleViewAction();
    toggle_ana_settings_wgt = ana_settings_dock->toggleViewAction();

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

    connect(analysis_wgt, SIGNAL(show_analysis_queue(bool)), this, SLOT(show_analysis_dock(bool)));
    connect(video_wgt, SIGNAL(start_analysis(VideoProject*, AnalysisSettings*)), project_wgt, SLOT(start_analysis(VideoProject*, AnalysisSettings*)));
    connect(video_wgt->frame_wgt, SIGNAL(quick_analysis(AnalysisSettings*)), video_wgt, SLOT(quick_analysis(AnalysisSettings*)));
    connect(project_wgt, SIGNAL(begin_analysis(QTreeWidgetItem*, AnalysisMethod*)),
            analysis_wgt, SLOT(start_analysis(QTreeWidgetItem*, AnalysisMethod*)));

    // Initialize bookmark widget
    bookmark_wgt = new BookmarkWidget();
    bookmark_wgt->setWindowFlags(Qt::Window);
    addDockWidget(Qt::RightDockWidgetArea, bookmark_dock);
    bookmark_dock->close();

    // Initialize drawing widget
    drawing_wgt = new DrawingWidget();
    drawing_dock->setWidget(drawing_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, drawing_dock);
    //connect(m_overlay, SIGNAL(new_drawing(Shapes*, int)), drawing_wgt, SLOT(add_drawing(Shapes*, int)));
    connect(drawing_wgt, SIGNAL(jump_to_frame(VideoProject*,int)), video_wgt, SLOT(load_marked_video(VideoProject*, int)));
    connect(drawing_wgt, SIGNAL(set_current_drawing(Shapes*)), video_wgt, SLOT(set_current_drawing(Shapes*)));
    connect(drawing_wgt, SIGNAL(delete_drawing(Shapes*)), this, SLOT(delete_drawing(Shapes*)));
    connect(drawing_wgt, SIGNAL(clear_frame(int)), this, SLOT(clear(int)));
    connect(drawing_wgt, SIGNAL(update_text(QString, Shapes*)), this, SLOT(update_text(QString, Shapes*)));
    connect(project_wgt, &ProjectWidget::save_draw_wgt, drawing_wgt, &DrawingWidget::save_item_data);
    
    // Initialize analysis queue widget
    queue_wgt = new QueueWidget();
    queue_dock->setWidget(queue_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, queue_dock);
    queue_dock->setFloating(true);
    queue_dock->close();
    analysis_wgt->set_queue_wgt(queue_wgt);

    connect(video_wgt, SIGNAL(new_bookmark(VideoProject*,int,cv::Mat)), bookmark_wgt, SLOT(create_bookmark(VideoProject*,int,cv::Mat)));
    connect(project_wgt, SIGNAL(proj_path(std::string)), bookmark_wgt, SLOT(set_path(std::string)));
    connect(project_wgt, SIGNAL(load_bookmarks(VideoProject*)), bookmark_wgt, SLOT(load_bookmarks(VideoProject*)));
    connect(bookmark_wgt, SIGNAL(play_bookmark_video(VideoProject*,int)), video_wgt, SLOT(load_marked_video(VideoProject*, int)));
    connect(project_wgt, &ProjectWidget::project_closed, bookmark_wgt, &BookmarkWidget::clear_bookmarks);
    bookmark_dock->setWidget(bookmark_wgt);

    // Initialize analysis settings widget
    ana_settings_wgt = new AnaSettingWidget(project_wgt->analysis_settings);
    ana_settings_dock->setWidget(ana_settings_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, ana_settings_dock);
    ana_settings_dock->setFloating(true);
    ana_settings_dock->close();

    connect(project_wgt, &ProjectWidget::update_settings_wgt, ana_settings_wgt, &AnaSettingWidget::set_ana_settings);
    connect(project_wgt, &ProjectWidget::show_analysis_settings, this, &MainWindow::show_ana_settings_dock);

    // Main toolbar
    MainToolbar* main_toolbar = new MainToolbar();
    main_toolbar->setWindowTitle(tr("Main toolbar"));
    addToolBar(main_toolbar);

    // Draw toolbar
    draw_toolbar = new DrawingToolbar();
    draw_toolbar->setWindowTitle(tr("Draw toolbar"));
    QAction* toggle_draw_toolbar = draw_toolbar->toggleViewAction();
    addToolBar(draw_toolbar);

    //Initialize menu bar
    init_file_menu();
    init_edit_menu();
    init_view_menu();
    init_analysis_menu();
    init_interval_menu();
    init_tools_menu();
    init_help_menu();

    // Toolbar connects
    connect(main_toolbar->add_video_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);
    connect(main_toolbar->save_act, &QAction::triggered, project_wgt, &ProjectWidget::save_project);
    connect(main_toolbar->open_act, &QAction::triggered, this, &MainWindow::open_project_dialog);
    //video_wgt->vertical_layout->insertWidget(0, draw_toolbar); <-- Add the toolbar to the 2nd video wgt
    connect(main_toolbar->toggle_draw_toolbar_act, &QAction::triggered, toggle_draw_toolbar, &QAction::trigger);   
    connect(draw_toolbar, SIGNAL(set_color(QColor)), video_wgt->frame_wgt, SLOT(set_overlay_color(QColor)));
    connect(draw_toolbar, SIGNAL(set_overlay_tool(SHAPES)), video_wgt->frame_wgt, SLOT(set_tool(SHAPES)));
    connect(draw_toolbar, SIGNAL(send_text(QString, float)), video_wgt, SLOT(set_tool_text(QString,float)));
    connect(draw_toolbar->delete_tool_act, &QAction::triggered, this, &MainWindow::delete_current_drawing);
    connect(color_act, &QAction::triggered, draw_toolbar, &DrawingToolbar::color_tool_clicked);
    connect(drawing_wgt, &DrawingWidget::set_tool_edit, draw_toolbar->edit_tool_act, &QAction::trigger);
    connect(drawing_wgt, &DrawingWidget::set_tool_zoom, draw_toolbar->zoom_tool_act, &QAction::trigger);
    connect(draw_toolbar, SIGNAL(step_zoom(double)), video_wgt, SLOT(on_step_zoom(double)));
    connect(video_wgt->frame_wgt, &FrameWidget::set_toolbar_zoom, draw_toolbar->zoom_tool_act, &QAction::trigger);

    draw_toolbar->zoom_tool_act->trigger();

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

    connect(project_wgt, &ProjectWidget::clear_analysis, video_wgt->frame_wgt, &FrameWidget::clear_analysis);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt->frame_wgt, &FrameWidget::clear_analysis);
    connect(video_wgt,   &VideoWidget::export_original_frame, bookmark_wgt, &BookmarkWidget::export_original_frame);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt->playback_slider, &AnalysisSlider::clear_slider);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt, &VideoWidget::load_marked_video);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt, &VideoWidget::clear_tag);
    connect(project_wgt, &ProjectWidget::marked_video, video_wgt->frame_wgt, &FrameWidget::set_video_project);
    connect(project_wgt, &ProjectWidget::marked_video, drawing_wgt, &DrawingWidget::set_video_project);

    connect(project_wgt, &ProjectWidget::project_closed, drawing_wgt, &DrawingWidget::clear_overlay);
    connect(project_wgt, &ProjectWidget::project_closed, video_wgt, &VideoWidget::clear_current_video);
    connect(project_wgt, SIGNAL(item_removed(VideoProject*)), video_wgt, SLOT(remove_item(VideoProject*)));

    connect(analysis_wgt, SIGNAL(name_in_tree(QTreeWidgetItem*,QString)), project_wgt, SLOT(set_tree_item_name(QTreeWidgetItem*,QString)));

    connect(project_wgt, SIGNAL(marked_analysis(AnalysisProxy*)), video_wgt->frame_wgt, SLOT(set_analysis(AnalysisProxy*)));
    connect(project_wgt, SIGNAL(marked_basic_analysis(BasicAnalysis*)), video_wgt->playback_slider, SLOT(set_basic_analysis(BasicAnalysis*)));
    connect(ana_details_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::set_show_ana_interval);
    connect(ana_details_act, &QAction::toggled, video_wgt->frame_wgt, &FrameWidget::show_bounding_box);
    connect(ana_details_act, SIGNAL(toggled(bool)), project_wgt, SLOT(toggle_details(bool)));
    connect(toggle_ana_settings_wgt, SIGNAL(toggled(bool)), project_wgt, SLOT(toggle_settings(bool)));
    connect(project_wgt, &ProjectWidget::toggle_analysis_details, ana_details_act, &QAction::toggle);
    connect(project_wgt, &ProjectWidget::toggle_settings_details, toggle_ana_settings_wgt, &QAction::trigger);

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
    rp_dialog = new RecentProjectDialog(this);
    rp_dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Create a new project if user presses "new project" or if dialog is rejected
    connect(rp_dialog, &RecentProjectDialog::new_project, project_wgt, &ProjectWidget::new_project);
    connect(rp_dialog, &RecentProjectDialog::rejected, project_wgt,&ProjectWidget::new_project);

    connect(rp_dialog, &RecentProjectDialog::open_project, project_wgt, &ProjectWidget::open_project);
    connect(rp_dialog, &RecentProjectDialog::open_project_from_file, project_wgt, &ProjectWidget::open_project);
    QTimer::singleShot(0, rp_dialog, SLOT(exec()));
}

/**
 * @brief MainWindow::~MainWindow
 * Destructor
 */
MainWindow::~MainWindow() {
    delete video_wgt;
    delete project_wgt;
    delete analysis_wgt;
    delete bookmark_wgt;
    delete status_bar;
    delete draw_toolbar;
    delete main_toolbar;
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
    new_project_act->setIcon(QIcon("../ViAn/Icons/new.png"));
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
    //remove_project_act->setShortcuts(QKeySequence::Delete); //Del
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
    connect(close_project_act, &QAction::triggered, project_wgt, &ProjectWidget::new_project);
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
    view_menu->addAction(toggle_queue_wgt);
    view_menu->addSeparator();
    view_menu->addAction(detect_intv_act);
    view_menu->addAction(bound_box_act);
    view_menu->addAction(interval_act);
    view_menu->addAction(drawing_act);

    toggle_project_wgt->setStatusTip(tr("Show/hide project widget"));
    toggle_bookmark_wgt->setStatusTip(tr("Show/hide bookmark widget"));
    toggle_queue_wgt->setStatusTip(tr("Show/hide analysis queue widget"));
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
    connect(drawing_act, &QAction::toggled, video_wgt, &VideoWidget::set_show_overlay);
}

/**
 * @brief MainWindow::init_analysis_menu
 * Set up the analysis menu
 */
void MainWindow::init_analysis_menu() {
    QMenu* analysis_menu = menuBar()->addMenu(tr("&Analysis"));

    QAction* advanced_analysis_act = new QAction(tr("&Full analysis"), this);
    QAction* quick_analysis_act = new QAction(tr("&ROI analysis"), this);
    QAction* settings_act = new QAction(tr("&Settings"), this);

    ana_details_act = new QAction(tr("Analysis &Details"), this);
    ana_details_act->setCheckable(true);
    ana_details_act->setChecked(false);

    advanced_analysis_act->setIcon(QIcon("../ViAn/Icons/advanced_analysis.png"));
    quick_analysis_act->setIcon(QIcon("../ViAn/Icons/analysis.png"));
    settings_act->setIcon(QIcon("../ViAn/Icons/cog.png"));

    advanced_analysis_act->setStatusTip(tr("Perform advanced analysis and select settings."));
    quick_analysis_act->setStatusTip(tr("Perform quick analysis on a custom area."));
    settings_act->setStatusTip(tr("Change the settings for analyses."));
    toggle_ana_settings_wgt->setStatusTip(tr("Show/hide analysis settings widget"));
    ana_details_act->setStatusTip(tr("Toggle analysis details on/off"));

    analysis_menu->addAction(quick_analysis_act);
    analysis_menu->addAction(advanced_analysis_act);
    analysis_menu->addAction(settings_act);
    analysis_menu->addSeparator();
    analysis_menu->addAction(toggle_ana_settings_wgt);
    analysis_menu->addAction(ana_details_act);

    connect(advanced_analysis_act, &QAction::triggered, project_wgt, &ProjectWidget::advanced_analysis);
    connect(quick_analysis_act, &QAction::triggered, draw_toolbar->analysis_tool_act, &QAction::trigger);
    connect(settings_act, &QAction::triggered, project_wgt, &ProjectWidget::update_analysis_settings);
}

void MainWindow::init_interval_menu() {
    QMenu* interval_menu = menuBar()->addMenu(tr("&Interval"));

    QAction* tag_interval_act = new QAction(tr("&Tag interval"), this);
    QAction* rm_tag_interval_act = new QAction(tr("&Remove tag on interval"), this);
    QAction* rm_interval_act = new QAction(tr("&Delete interval"), this);

    tag_interval_act->setShortcut(tr("Shift+T"));
    rm_tag_interval_act->setShortcut(tr("Shift+R"));
    rm_interval_act->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Delete));

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

    QAction* zoom_in_act = new QAction(tr("&Zoom in"), this);
    color_act = new QAction(tr("&Color"), this);
    QAction* zoom_out_act = new QAction(tr("Zoom &out"), this);
    QAction* fit_screen_act = new QAction(tr("&Fit to screen"), this);
    QAction* reset_zoom_act = new QAction(tr("Re&set zoom"), this);
    QAction* rectangle_act = new QAction(tr("&Rectangle"), this);
    QAction* circle_act = new QAction(tr("C&ircle"), this);
    QAction* line_act = new QAction(tr("Li&ne"), this);
    QAction* arrow_act = new QAction(tr("&Arrow"), this);
    QAction* pen_act = new QAction(tr("&Pen"), this);
    QAction* text_act = new QAction(tr("&Text"), this);

    QAction* export_act = new QAction(tr("&Export interval"), this);
    export_act->setShortcut(tr("Shift+E"));
    QAction* clear_frame_act = new QAction(tr("C&lear"), this);
    QAction* delete_drawing_act = new QAction(tr("&Delete drawing"), this);

    color_act->setIcon(QIcon("../ViAn/Icons/color.png"));
    zoom_in_act->setIcon(QIcon("../ViAn/Icons/zoom_in.png"));
    zoom_out_act->setIcon(QIcon("../ViAn/Icons/zoom_out.png"));
    fit_screen_act->setIcon(QIcon("../ViAn/Icons/fit_screen.png"));
    reset_zoom_act->setIcon(QIcon("../ViAn/Icons/move.png"));
    rectangle_act->setIcon(QIcon("../ViAn/Icons/box.png"));
    circle_act->setIcon(QIcon("../ViAn/Icons/circle.png"));
    line_act->setIcon(QIcon("../ViAn/Icons/line.png"));
    arrow_act->setIcon(QIcon("../ViAn/Icons/arrow.png"));
    pen_act->setIcon(QIcon("../ViAn/Icons/pen.png"));
    text_act->setIcon(QIcon("../ViAn/Icons/text.png"));
    clear_frame_act->setIcon(QIcon("../ViAn/Icons/clear.png"));
    delete_drawing_act->setIcon(QIcon("../ViAn/Icons/clear.png"));

    // Export submenu
    QMenu* export_menu = tool_menu->addMenu(tr("&Export"));
    export_menu->addAction(export_act);

    tool_menu->addSeparator();

    tool_menu->addAction(color_act);
    QMenu* drawing_tools = tool_menu->addMenu(tr("&Shapes"));
    drawing_tools->addAction(rectangle_act);
    drawing_tools->addAction(circle_act);
    drawing_tools->addAction(line_act);
    drawing_tools->addAction(arrow_act);
    drawing_tools->addAction(pen_act);
    drawing_tools->addAction(text_act);

    tool_menu->addAction(delete_drawing_act);
    tool_menu->addAction(clear_frame_act);
    tool_menu->addSeparator();
    tool_menu->addAction(zoom_in_act);
    tool_menu->addAction(zoom_out_act);
    tool_menu->addAction(fit_screen_act);
    tool_menu->addAction(reset_zoom_act);

    clear_frame_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));

    color_act->setStatusTip(tr("Color picker"));
    zoom_in_act->setStatusTip(tr("Zoom in"));
    zoom_out_act->setStatusTip(tr("Zoom out"));
    fit_screen_act->setStatusTip(tr("Fit to screen"));
    reset_zoom_act->setStatusTip(tr("Reset zoom"));
    rectangle_act->setStatusTip(tr("Rectangle tool"));
    circle_act->setStatusTip(tr("Circle tool"));
    line_act->setStatusTip(tr("Line tool"));
    arrow_act->setStatusTip(tr("Arrow tool"));
    pen_act->setStatusTip(tr("Pen tool"));
    text_act->setStatusTip(tr("Text tool"));
    clear_frame_act->setStatusTip(tr("Clear all drawings on the current frame"));
    delete_drawing_act->setStatusTip(tr("Delete the current drawing"));

    //Connect
    connect(export_act, &QAction::triggered, this, &MainWindow::export_images);
    connect(fit_screen_act, &QAction::triggered, video_wgt, &VideoWidget::on_fit_screen);
    connect(reset_zoom_act, &QAction::triggered, video_wgt, &VideoWidget::on_original_size);
    connect(rectangle_act, &QAction::triggered, draw_toolbar->rectangle_tool_act, &QAction::trigger);
    connect(circle_act, &QAction::triggered, draw_toolbar->circle_tool_act, &QAction::trigger);
    connect(line_act, &QAction::triggered, draw_toolbar->line_tool_act, &QAction::trigger);
    connect(arrow_act, &QAction::triggered, draw_toolbar->arrow_tool_act, &QAction::trigger);
    connect(pen_act, &QAction::triggered, draw_toolbar->pen_tool_act, &QAction::trigger);
    connect(text_act, &QAction::triggered, draw_toolbar->text_tool_act, &QAction::trigger);
    connect(clear_frame_act, &QAction::triggered, this, &MainWindow::clear_current);
    connect(delete_drawing_act, &QAction::triggered, draw_toolbar->delete_tool_act, &QAction::trigger);
    connect(zoom_in_act, &QAction::triggered, draw_toolbar->zoom_in_tool_act, &QAction::trigger);
    connect(zoom_out_act, &QAction::triggered, draw_toolbar->zoom_out_tool_act, &QAction::trigger);
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

void MainWindow::closeEvent(QCloseEvent *event) {
    if (project_wgt->close_project()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::update_text(QString text, Shapes* shape) {
    video_wgt->set_update_text(text, shape);
}

void MainWindow::clear(int frame) {
    video_wgt->set_clear_drawings(frame);
}

void MainWindow::clear_current() {
    video_wgt->set_clear_drawings(-1);
}

void MainWindow::delete_drawing(Shapes *shape) {
    video_wgt->set_delete_drawing(shape);
}

void MainWindow::delete_current_drawing() {
    drawing_wgt->delete_item();
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
    ManipulatorDialog* man_dialog = new ManipulatorDialog(video_wgt->get_brightness(), video_wgt->get_contrast(), this);
    man_dialog->setAttribute(Qt::WA_DeleteOnClose);
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
    FrameExporterDialog exporter_dialog(im_exp, vid_proj->get_video(), project_wgt->m_proj->get_dir(),
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
    QString project_path = QFileDialog().getOpenFileName(
                this,
                tr("Open project"),
                QDir::homePath(),
                "*.vian");
    emit open_project(project_path);
}

void MainWindow::show_analysis_dock(bool show) {
    if (show) {
        queue_dock->show();
    } else if (queue_dock->isFloating()) {
        queue_dock->close();
    }
}

void MainWindow::show_ana_settings_dock(bool show) {
    if (show) {
        ana_settings_dock->show();
    }
}
