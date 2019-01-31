#include "mainwindow.h"

#include "Analysis/analysismethod.h"
#include "Analysis/analysisslider.h"
#include "constants.h"
#include "framewidget.h"
#include "GUI/Analysis/analysiswidget.h"
#include "GUI/Analysis/anasettingwidget.h"
#include "GUI/Analysis/queuewidget.h"
#include "GUI/Bookmark/bookmarkwidget.h"
#include "GUI/drawingwidget.h"
#include "GUI/frameexporterdialog.h"
#include "GUI/manipulatorwidget.h"
#include "GUI/projectwidget.h"
#include "GUI/recentprojectdialog.h"
#include "GUI/settingsdialog.h"
#include "GUI/viewpathdialog.h"
#include "GUI/zoompreviewwidget.h"
#include "imageexporter.h"
#include "Project/Analysis/analysisproxy.h"
#include "Project/Analysis/interval.h"
#include "Project/Analysis/tag.h"
#include "Project/Analysis/tagframe.h"
#include "Project/project.h"
#include "Project/videoproject.h"
#include "Toolbars/drawingtoolbar.h"
#include "Toolbars/maintoolbar.h"
#include "statusbar.h"
#include "videowidget.h"


#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QDockWidget>
#include <QFileDialog>
#include <QListWidget>
#include <QMenuBar>
#include <QProgressDialog>
#include <QThread>
#include <QTimer>

/**
 * @brief MainWindow::MainWindow
 * @param parent a QWidget variable
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QDockWidget* project_dock = new QDockWidget(tr("Projects"), this);
    QDockWidget* drawing_dock = new QDockWidget(tr("Drawings"), this);
    bookmark_dock = new QDockWidget(tr("Bookmarks"), this);
    QDockWidget* zoom_preview_dock = new QDockWidget(tr("Zoom preview"), this);
    queue_dock = new QDockWidget(tr("Analysis queue"), this);
    ana_settings_dock = new QDockWidget(tr("Analysis settings"), this);
    manipulator_dock = new QDockWidget(tr("Color correction settings"), this);
    project_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    drawing_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    bookmark_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    zoom_preview_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    queue_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    ana_settings_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    manipulator_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toggle_project_wgt = project_dock->toggleViewAction();
    toggle_drawing_wgt = drawing_dock->toggleViewAction();
    toggle_bookmark_wgt = bookmark_dock->toggleViewAction();
    toggle_queue_wgt = queue_dock->toggleViewAction();
    toggle_ana_settings_wgt = ana_settings_dock->toggleViewAction();
    toggle_zoom_preview_wgt = zoom_preview_dock->toggleViewAction();
    toggle_manipulator_wgt = manipulator_dock->toggleViewAction();

    // Initialize video widget
    video_wgt = new VideoWidget();
    video_wgt->setMinimumSize(Constants::VIDEO_WGT_WIDTH * Constants::SIZE_MULTIPLIER,
                              Constants::VIDEO_WGT_HEIGHT * Constants::SIZE_MULTIPLIER); // width and height sets aspect ratio
    setCentralWidget(video_wgt);

    // Initialize project widget
    project_wgt = new ProjectWidget();
    project_dock->setWidget(project_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, project_dock);
    project_wgt->new_project();
    project_wgt->setIconSize(Singleton::get_instance()->PROJ_THUMBNAIL_SIZE);

    connect(project_wgt, &ProjectWidget::open_in_widget, this, &MainWindow::open_widget);
    connect(project_wgt, &ProjectWidget::close_all_widgets, this, &MainWindow::close_all_widgets);

    // Initialize analysis widget
    analysis_wgt = new AnalysisWidget();
    connect(analysis_wgt, &AnalysisWidget::show_analysis_queue, this, &MainWindow::show_analysis_dock);
    connect(video_wgt, &VideoWidget::start_analysis, project_wgt, &ProjectWidget::start_analysis);
    connect(video_wgt->frame_wgt, &FrameWidget::quick_analysis, video_wgt, &VideoWidget::quick_analysis);
    connect(project_wgt, &ProjectWidget::begin_analysis, analysis_wgt, &AnalysisWidget::start_analysis);
    connect(project_wgt, &ProjectWidget::abort_all_analysis, analysis_wgt, &AnalysisWidget::abort_all_analysis);

    // Initialize drawing widget
    drawing_wgt = new DrawingWidget();
    drawing_dock->setWidget(drawing_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, drawing_dock);
    connect(drawing_wgt, &DrawingWidget::jump_to_frame, video_wgt, &VideoWidget::load_marked_video_state);
    connect(drawing_wgt, &DrawingWidget::set_current_drawing, video_wgt, &VideoWidget::set_current_drawing);
    connect(drawing_wgt, &DrawingWidget::delete_drawing, this, &MainWindow::delete_drawing);
    connect(drawing_wgt, &DrawingWidget::clear_frame, this, &MainWindow::clear);
    connect(drawing_wgt, &DrawingWidget::update_text, this, &MainWindow::update_text);

    connect(project_wgt, &ProjectWidget::save_draw_wgt, drawing_wgt, &DrawingWidget::save_item_data);
    connect(video_wgt, &VideoWidget::delete_sc_activated, drawing_wgt, &DrawingWidget::delete_item);

    // Initialize bookmark widget
    bookmark_wgt = new BookmarkWidget();
    bookmark_wgt->setWindowFlags(Qt::Window);
    addDockWidget(Qt::RightDockWidgetArea, bookmark_dock);
    bookmark_dock->close();

    // Initialize zoom preview widget
    zoom_wgt = new ZoomPreviewWidget();
    zoom_preview_dock->setWidget(zoom_wgt);
    addDockWidget(Qt::RightDockWidgetArea, zoom_preview_dock);

    connect(video_wgt, &VideoWidget::clean_zoom_preview, zoom_wgt, &ZoomPreviewWidget::clean_zoom_widget);
    connect(video_wgt, &VideoWidget::zoom_preview, zoom_wgt, &ZoomPreviewWidget::frame_update);
    connect(zoom_wgt, &ZoomPreviewWidget::window_size, video_wgt, &VideoWidget::update_zoom_preview_size);
    connect(zoom_wgt, &ZoomPreviewWidget::pan_translation, video_wgt, &VideoWidget::translate_zoom_from_preview_click);
    connect(zoom_preview_dock, &QDockWidget::topLevelChanged, zoom_wgt, &ZoomPreviewWidget::on_floating_changed);
    
    // Initialize analysis queue widget
    queue_wgt = new QueueWidget();
    queue_dock->setWidget(queue_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, queue_dock);
    queue_dock->setFloating(true);
    queue_dock->close();
    analysis_wgt->set_queue_wgt(queue_wgt);

    connect(video_wgt, &VideoWidget::new_bookmark, bookmark_wgt, &BookmarkWidget::create_bookmark);
    connect(bookmark_wgt, &BookmarkWidget::show_bm_dock, this, &MainWindow::show_bookmark_dock);
    connect(project_wgt, &ProjectWidget::set_project, bookmark_wgt, &BookmarkWidget::set_project);
    connect(bookmark_wgt, &BookmarkWidget::play_bookmark_video, video_wgt, &VideoWidget::load_marked_video_state);
    connect(project_wgt, &ProjectWidget::project_closed, bookmark_wgt, &BookmarkWidget::clear_bookmarks);
    connect(project_wgt, &ProjectWidget::save_bmark_wgt, bookmark_wgt, &BookmarkWidget::save_item_data);
    bookmark_dock->setWidget(bookmark_wgt);

    // Initialize analysis settings widget
    ana_settings_wgt = new AnaSettingWidget(project_wgt->analysis_settings);
    ana_settings_dock->setWidget(ana_settings_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, ana_settings_dock);
    ana_settings_dock->setFloating(true);
    ana_settings_dock->close();

    connect(project_wgt, &ProjectWidget::update_settings_wgt, ana_settings_wgt, &AnaSettingWidget::set_ana_settings);
    connect(project_wgt, &ProjectWidget::show_analysis_settings, this, &MainWindow::show_ana_settings_dock);

    // Initialize manipulator widget
    manipulator_wgt = new ManipulatorWidget();
    manipulator_dock->setWidget(manipulator_wgt);
    addDockWidget(Qt::LeftDockWidgetArea, manipulator_dock);
    manipulator_dock->setFloating(true);
    manipulator_dock->close();

    connect(manipulator_wgt, &ManipulatorWidget::values, video_wgt, &VideoWidget::update_brightness_contrast);
    connect(manipulator_wgt, &ManipulatorWidget::update_tag, video_wgt, &VideoWidget::update_tag_color);

    // Main toolbar
    main_toolbar = new MainToolbar();
    main_toolbar->setWindowTitle(tr("Main toolbar"));
    toggle_main_toolbar = main_toolbar->toggleViewAction();
    addToolBar(main_toolbar);

    // Draw toolbar
    draw_toolbar = new DrawingToolbar();
    draw_toolbar->setWindowTitle(tr("Draw toolbar"));
    toggle_drawing_toolbar = draw_toolbar->toggleViewAction();
    addToolBar(draw_toolbar);

    // Recent projects menu
    init_rp_dialog();

    //Initialize menu bar
    init_file_menu();
    init_edit_menu();
    init_view_menu();
    init_analysis_menu();
    init_interval_menu();
    init_drawings_menu();
    init_export_menu();
    init_help_menu();

    // Clicking toolbar connects

    // Main toolbar
    connect(main_toolbar->save_act, &QAction::triggered, project_wgt, &ProjectWidget::save_project);
    connect(main_toolbar->open_act, &QAction::triggered, this, &MainWindow::open_project_dialog);
    connect(main_toolbar->add_video_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);
    connect(main_toolbar->add_img_seq_act, &QAction::triggered, project_wgt, &ProjectWidget::add_images);
    connect(main_toolbar->open_recent_act, &QAction::triggered, this, &MainWindow::open_rp_dialog);
    connect(main_toolbar->open_folder_act, &QAction::triggered, this, &MainWindow::open_project_folder);

    // Drawing toolbar
    connect(draw_toolbar, &DrawingToolbar::step_zoom, video_wgt, &VideoWidget::on_step_zoom);
    connect(draw_toolbar, &DrawingToolbar::set_overlay_tool, video_wgt->frame_wgt, &FrameWidget::set_tool);;
    connect(draw_toolbar, &DrawingToolbar::send_text, video_wgt, &VideoWidget::set_tool_text);
    connect(draw_toolbar, &DrawingToolbar::set_color, video_wgt->frame_wgt, &FrameWidget::set_overlay_color);
    connect(draw_toolbar->delete_tool_act, &QAction::triggered, this, &MainWindow::delete_current_drawing);

    // Update the toolbar from other places
    connect(drawing_wgt, &DrawingWidget::set_tool_edit, draw_toolbar->edit_tool_act, &QAction::trigger);
    connect(drawing_wgt, &DrawingWidget::set_tool_zoom, draw_toolbar->zoom_tool_act, &QAction::trigger);
    connect(video_wgt, &VideoWidget::set_zoom_tool, draw_toolbar->zoom_tool_act, &QAction::trigger);
    connect(project_wgt, &ProjectWidget::set_zoom_tool, draw_toolbar->zoom_tool_act, &QAction::trigger);
    connect(video_wgt->frame_wgt, &FrameWidget::set_toolbar_zoom, draw_toolbar->zoom_tool_act, &QAction::trigger);
    connect(color_act, &QAction::triggered, draw_toolbar, &DrawingToolbar::color_tool_clicked);

    draw_toolbar->zoom_tool_act->trigger();

    // Status bar
    status_bar = new StatusBar();
    setStatusBar(status_bar);
    connect(this, &MainWindow::set_status_bar, status_bar, &StatusBar::on_set_status_bar);
    connect(video_wgt, &VideoWidget::set_status_bar, status_bar, &StatusBar::on_set_status_bar);
    connect(project_wgt, &ProjectWidget::set_status_bar, status_bar, &StatusBar::on_set_status_bar);
    connect(bookmark_wgt, &BookmarkWidget::set_status_bar, status_bar, &StatusBar::on_set_status_bar);
    connect(draw_toolbar, &DrawingToolbar::set_status_bar, status_bar, &StatusBar::on_set_status_bar);
    connect(analysis_wgt, &AnalysisWidget::add_analysis_bar, status_bar, &StatusBar::add_analysis_bar);
    connect(analysis_wgt, &AnalysisWidget::remove_analysis_bar, status_bar, &StatusBar::remove_analysis_bar);
    connect(analysis_wgt, &AnalysisWidget::show_progress, status_bar, &StatusBar::update_analysis_bar);

    // Connects for settings video project, analysis etc when clicking the project tree
    connect(project_wgt, &ProjectWidget::set_video_project, video_wgt->frame_wgt, &FrameWidget::set_video_project);
    connect(project_wgt, &ProjectWidget::set_video_project, drawing_wgt, &DrawingWidget::set_video_project);
    connect(project_wgt, &ProjectWidget::marked_video_state, video_wgt, &VideoWidget::load_marked_video_state);
    connect(project_wgt, &ProjectWidget::marked_analysis, video_wgt->frame_wgt, &FrameWidget::set_analysis);
    connect(project_wgt, &ProjectWidget::marked_analysis, video_wgt->playback_slider, &AnalysisSlider::set_analysis_proxy);
    connect(project_wgt, &ProjectWidget::marked_basic_analysis, video_wgt->playback_slider, &AnalysisSlider::set_basic_analysis);
    connect(project_wgt, &ProjectWidget::marked_basic_analysis, video_wgt, &VideoWidget::set_basic_analysis);

    // Connects for showing/hiding widgets and info
    connect(ana_details_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::set_details_checked);
    connect(ana_details_act, &QAction::toggled, video_wgt->frame_wgt, &FrameWidget::set_details_checked);
    connect(ana_details_act, &QAction::toggled, project_wgt, &ProjectWidget::toggle_details);
    connect(toggle_ana_settings_wgt, &QAction::toggled, project_wgt, &ProjectWidget::toggle_settings);
    connect(project_wgt, &ProjectWidget::toggle_analysis_details, ana_details_act, &QAction::toggle);
    connect(project_wgt, &ProjectWidget::toggle_settings_details, toggle_ana_settings_wgt, &QAction::trigger);
    connect(project_wgt, &ProjectWidget::set_show_analysis_details, video_wgt->frame_wgt, &FrameWidget::set_show_box);
    connect(project_wgt, &ProjectWidget::set_show_analysis_details, video_wgt->playback_slider, &AnalysisSlider::set_show_ana_interval);
    connect(project_wgt, &ProjectWidget::set_detections, video_wgt->frame_wgt, &FrameWidget::set_detections);
    connect(project_wgt, &ProjectWidget::enable_poi_btns, video_wgt, &VideoWidget::enable_poi_btns);
    connect(project_wgt, &ProjectWidget::seq_tag_btns, video_wgt, &VideoWidget::set_seq_tag_btns);
    connect(project_wgt, &ProjectWidget::set_poi_slider, video_wgt->playback_slider, &AnalysisSlider::set_show_pois);
    connect(project_wgt, &ProjectWidget::set_tag_slider, video_wgt->playback_slider, &AnalysisSlider::set_show_tags);
    connect(project_wgt, &ProjectWidget::set_interval_slider, video_wgt->playback_slider, &AnalysisSlider::set_show_interval_areas);

    // Tag connects
    connect(video_wgt, &VideoWidget::add_tag, project_wgt, &ProjectWidget::add_tag);
    connect(video_wgt, &VideoWidget::tag_new_frame, project_wgt, &ProjectWidget::add_new_frame_to_tag_item);
    connect(video_wgt, &VideoWidget::tag_remove_frame, project_wgt, &ProjectWidget::remove_frame_from_tag_item);
    connect(project_wgt, &ProjectWidget::update_tag, video_wgt, &VideoWidget::update_tag);
    connect(project_wgt, &ProjectWidget::clear_tag, video_wgt, &VideoWidget::clear_tag);

    // Interval connects
    connect(video_wgt, &VideoWidget::add_interval, project_wgt, &ProjectWidget::add_interval);
    connect(video_wgt, &VideoWidget::set_interval_area, project_wgt, &ProjectWidget::set_interval_area);

    // Connects for removing, clearing or closing
    connect(project_wgt, &ProjectWidget::clear_analysis, video_wgt->frame_wgt, &FrameWidget::clear_analysis);
    connect(project_wgt, &ProjectWidget::clear_slider, video_wgt->playback_slider, &AnalysisSlider::clear_slider);
    connect(project_wgt, &ProjectWidget::project_closed, drawing_wgt, &DrawingWidget::clear_overlay);
    connect(project_wgt, &ProjectWidget::project_closed, video_wgt, &VideoWidget::clear_current_video);
    connect(project_wgt, &ProjectWidget::item_removed, video_wgt, &VideoWidget::remove_item);
    connect(project_wgt, &ProjectWidget::remove_overlay, video_wgt, &VideoWidget::set_overlay_removed);

    // Other connects
    connect(project_wgt, &ProjectWidget::update_slider, video_wgt->playback_slider, &AnalysisSlider::update);
    connect(project_wgt, &ProjectWidget::item_type, video_wgt, &VideoWidget::set_item_type);
    connect(video_wgt, &VideoWidget::update_manipulator_wgt, manipulator_wgt, &ManipulatorWidget::set_values);
    connect(video_wgt, &VideoWidget::export_original_frame, bookmark_wgt, &BookmarkWidget::export_original_frame);
    connect(analysis_wgt, &AnalysisWidget::name_in_tree, project_wgt, &ProjectWidget::set_tree_item_name);
    connect(video_wgt, &VideoWidget::open_view_path_dialog, this, &MainWindow::view_paths);
    connect(video_wgt, &VideoWidget::update_videoitem, project_wgt, &ProjectWidget::update_current_videoitem);
    connect(project_wgt, &ProjectWidget::new_slider_max, video_wgt, &VideoWidget::set_slider_max);

    // Open the recent project dialog
    QTimer::singleShot(0, rp_dialog, &RecentProjectDialog::exec);
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
}

/**
 * @brief MainWindow::init_file_menu
 * Set up the file menu
 */
void MainWindow::init_file_menu() {
    QMenu* file_menu = menuBar()->addMenu(tr("&File"));

    // Init actions
    QAction* new_project_act = new QAction(tr("&New project"), this);
    QAction* open_project_act = new QAction(tr("&Open project..."), this);
    QAction* recent_project_act = new QAction(tr("&Recent projects..."), this);
    QAction* open_proj_folder_act = new QAction(tr("&Open project folder..."), this);
    QAction* save_project_act = new QAction(tr("&Save project"), this);
    QAction* add_vid_act = new QAction(tr("&Import video..."), this);
    QAction* add_seq_act = new QAction(tr("&Import images"), this);
    QAction* view_paths = new QAction(tr("&View paths"), this);
    QAction* quit_act = new QAction(tr("&Quit"), this);

    // Set icons
    new_project_act->setIcon(QIcon("../ViAn/Icons/new.png"));
    open_project_act->setIcon(QIcon("../ViAn/Icons/open.png"));
    recent_project_act->setIcon(QIcon("../ViAn/Icons/home.png"));
    open_proj_folder_act->setIcon(QIcon("../ViAn/Icons/new_folder.png"));
    save_project_act->setIcon(QIcon("../ViAn/Icons/save.png"));
    add_vid_act->setIcon(QIcon("../ViAn/Icons/add_video.png"));
    add_seq_act->setIcon(QIcon("../ViAn/Icons/image_sequence.png"));
    view_paths->setIcon(QIcon("../ViAn/Icons/path.png"));
    quit_act->setIcon(QIcon("../ViAn/Icons/quit.png"));

    // Add actions to the menu
    file_menu->addAction(new_project_act);
    file_menu->addAction(open_project_act);
    file_menu->addAction(recent_project_act);
    file_menu->addAction(open_proj_folder_act);
    file_menu->addAction(save_project_act);
    file_menu->addAction(add_vid_act);
    file_menu->addAction(add_seq_act);
    file_menu->addSeparator();
    file_menu->addAction(view_paths);
    file_menu->addSeparator();
    file_menu->addAction(quit_act);

    // Set shortcuts
    new_project_act->setShortcut(QKeySequence::New);       //Ctrl + N
    open_project_act->setShortcut(QKeySequence::Open);     //Ctrl + O
    recent_project_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    open_proj_folder_act->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_O));
    save_project_act->setShortcut(QKeySequence::Save);     //Ctrl + S
    add_vid_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));     //Ctrl + I
    // TODO    add_seq_act->setShortcuts(QKeySequence::SelectAll);     //Ctrl + A
    // TODO    view_paths->setShortcuts(Q);     //Ctrl + smth
    quit_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));

    // Set status tips
    new_project_act->setStatusTip(tr("Create a new project"));
    open_project_act->setStatusTip(tr("Load project"));
    recent_project_act->setStatusTip(tr("Open the recent project dialog"));
    open_proj_folder_act->setStatusTip(tr("Go to the project folder"));
    save_project_act->setStatusTip(tr("Save project"));
    add_vid_act->setStatusTip(tr("Add video"));
    add_vid_act->setStatusTip(tr("Import images"));
    view_paths->setStatusTip(tr("View all paths in project"));
    quit_act->setStatusTip(tr("Quit the application"));

    // Connect with signals and slots
    connect(new_project_act, &QAction::triggered, project_wgt, &ProjectWidget::new_project);
    connect(open_project_act, &QAction::triggered, this, &MainWindow::open_project_dialog);
    connect(recent_project_act, &QAction::triggered, this, &MainWindow::open_rp_dialog);
    connect(open_proj_folder_act, &QAction::triggered, this, &MainWindow::open_project_folder);
    connect(save_project_act, &QAction::triggered, project_wgt, &ProjectWidget::save_project);
    connect(add_vid_act, &QAction::triggered, project_wgt, &ProjectWidget::add_video);
    connect(add_seq_act, &QAction::triggered, project_wgt, &ProjectWidget::add_images);
    connect(view_paths, &QAction::triggered, this, &MainWindow::view_paths);
    connect(quit_act, &QAction::triggered, this, &QWidget::close);
}

/**
 * @brief MainWindow::init_edit_menu
 * Set up the edit menu
 */
void MainWindow::init_edit_menu() {
    QMenu* edit_menu = menuBar()->addMenu(tr("&Edit"));

    QAction* cont_bri_act = new QAction(tr("&Color corrections..."), this);
    QAction* cw_act = new QAction(tr("&Rotate 90°"), this);
    QAction* ccw_act = new QAction(tr("&Rotate 90°"), this);
    QAction* zoom_in_act = new QAction(tr("&Zoom in"), this);
    QAction* zoom_out_act = new QAction(tr("&Zoom out"), this);
    QAction* fit_screen_act = new QAction(tr("&Fit to screen"), this);
    QAction* reset_zoom_act = new QAction(tr("&Reset zoom"), this);
    QAction* options_act = new QAction(tr("&Options..."), this);

    cont_bri_act->setIcon(QIcon("../ViAn/Icons/screen.png"));
    cw_act->setIcon(QIcon("../ViAn/Icons/right.png"));
    ccw_act->setIcon(QIcon("../ViAn/Icons/left.png"));
    zoom_in_act->setIcon(QIcon("../ViAn/Icons/zoom_in.png"));
    zoom_out_act->setIcon(QIcon("../ViAn/Icons/zoom_out.png"));
    fit_screen_act->setIcon(QIcon("../ViAn/Icons/resize.png"));
    reset_zoom_act->setIcon(QIcon("../ViAn/Icons/original_size.png"));
    options_act->setIcon(QIcon("../ViAn/Icons/gears.png"));

    edit_menu->addAction(cont_bri_act);
    edit_menu->addAction(cw_act);
    edit_menu->addAction(ccw_act);
    edit_menu->addAction(zoom_in_act);
    edit_menu->addAction(zoom_out_act);
    edit_menu->addAction(fit_screen_act);
    edit_menu->addAction(reset_zoom_act);
    edit_menu->addSeparator();
    edit_menu->addAction(options_act);

    cont_bri_act->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_G));
    cw_act->setShortcut(QKeySequence(Qt::Key_R));
    ccw_act->setShortcut(QKeySequence(Qt::Key_L));
    zoom_in_act->setShortcut(Qt::Key_Plus);
    zoom_out_act->setShortcut(Qt::Key_Minus);
    fit_screen_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    reset_zoom_act->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));

    cont_bri_act->setStatusTip(tr("Open a color correction dialog"));
    cw_act->setStatusTip(tr("Rotate clockwise"));
    ccw_act->setStatusTip(tr("Rotate counter clockwise"));
    zoom_in_act->setStatusTip(tr("Zoom in"));
    zoom_out_act->setStatusTip(tr("Zoom out"));
    fit_screen_act->setStatusTip(tr("Fit to screen"));
    reset_zoom_act->setStatusTip(tr("Reset zoom"));
    options_act->setStatusTip(tr("Program options"));

    connect(cont_bri_act, &QAction::triggered, this, &MainWindow::cont_bri);
    connect(cw_act, &QAction::triggered, video_wgt, &VideoWidget::rotate_cw);
    connect(ccw_act, &QAction::triggered, video_wgt, &VideoWidget::rotate_ccw);
    connect(zoom_in_act, &QAction::triggered, draw_toolbar->zoom_in_tool_act, &QAction::trigger);
    connect(zoom_out_act, &QAction::triggered, draw_toolbar->zoom_out_tool_act, &QAction::trigger);
    connect(fit_screen_act, &QAction::triggered, video_wgt, &VideoWidget::on_fit_screen);
    connect(reset_zoom_act, &QAction::triggered, video_wgt, &VideoWidget::on_original_size);
    connect(options_act, &QAction::triggered, this, &MainWindow::options);
}

/**
 * @brief MainWindow::init_view_menu
 * Set up the view menu
 */
void MainWindow::init_view_menu() {
    QMenu* view_menu = menuBar()->addMenu(tr("&Windows"));

    view_menu->addAction(toggle_project_wgt);
    view_menu->addAction(toggle_drawing_wgt);
    view_menu->addAction(toggle_bookmark_wgt);
    view_menu->addAction(toggle_zoom_preview_wgt);
    view_menu->addAction(toggle_queue_wgt);
    view_menu->addAction(toggle_ana_settings_wgt);
    view_menu->addAction(toggle_manipulator_wgt);
    view_menu->addSeparator();
    view_menu->addAction(toggle_main_toolbar);
    view_menu->addAction(toggle_drawing_toolbar);

    toggle_project_wgt->setStatusTip(tr("Show/hide project widget"));
    toggle_drawing_wgt->setStatusTip(tr("Show/hide drawing widget"));
    toggle_bookmark_wgt->setStatusTip(tr("Show/hide bookmark widget"));
    toggle_zoom_preview_wgt->setStatusTip(tr("Show/hide zoom preview"));
    toggle_queue_wgt->setStatusTip(tr("Show/hide analysis queue widget"));
    toggle_ana_settings_wgt->setStatusTip(tr("Show/hide analysis info widget"));
    toggle_manipulator_wgt->setStatusTip(tr("Show/hide color correction widget"));
}

/**
 * @brief MainWindow::init_analysis_menu
 * Set up the analysis menu
 */
void MainWindow::init_analysis_menu() {
    QMenu* analysis_menu = menuBar()->addMenu(tr("&Analysis"));

    QAction* quick_analysis_act = new QAction(tr("&ROI analysis"), this);
    QAction* advanced_analysis_act = new QAction(tr("&Full analysis..."), this);
    QAction* settings_act = new QAction(tr("Analysis &settings..."), this);
    ana_details_act = new QAction(tr("&Analysis details"), this);
    detect_intv_act = new QAction(tr("&Detection intervals"), this);      //Slider pois
    bound_box_act = new QAction(tr("&Bounding boxes"), this);        //Video oois

    ana_details_act->setCheckable(true);
    detect_intv_act->setCheckable(true);
    bound_box_act->setCheckable(true);

    ana_details_act->setChecked(false);
    detect_intv_act->setChecked(true);
    bound_box_act->setChecked(true);

    advanced_analysis_act->setIcon(QIcon("../ViAn/Icons/advanced_analys.png"));
    quick_analysis_act->setIcon(QIcon("../ViAn/Icons/analys.png"));
    settings_act->setIcon(QIcon("../ViAn/Icons/cog.png"));

    quick_analysis_act->setStatusTip(tr("Perform quick analysis on a custom area."));
    advanced_analysis_act->setStatusTip(tr("Perform advanced analysis and select settings."));
    settings_act->setStatusTip(tr("Change the settings for analyses."));
    ana_details_act->setStatusTip(tr("Toggle analysis details on/off"));
    detect_intv_act->setStatusTip(tr("Toggle notations on slider on/off"));
    bound_box_act->setStatusTip(tr("Toggle boxes on video on/off"));

    //quick_analysis_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));   // Not set because it collides with the alt menu shortcuts
    advanced_analysis_act->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_A));

    analysis_menu->addAction(quick_analysis_act);
    analysis_menu->addAction(advanced_analysis_act);
    analysis_menu->addAction(settings_act);
    analysis_menu->addSeparator();
    analysis_menu->addAction(ana_details_act);
    analysis_menu->addAction(detect_intv_act);
    analysis_menu->addAction(bound_box_act);

    connect(advanced_analysis_act, &QAction::triggered, project_wgt, &ProjectWidget::advanced_analysis);
    connect(quick_analysis_act, &QAction::triggered, draw_toolbar->analysis_tool_act, &QAction::trigger);
    connect(settings_act, &QAction::triggered, project_wgt, &ProjectWidget::update_analysis_settings);
    connect(bound_box_act, &QAction::toggled, video_wgt->frame_wgt, &FrameWidget::set_show_detections);
    connect(bound_box_act, &QAction::toggled, video_wgt->frame_wgt, &FrameWidget::update);
    connect(detect_intv_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::set_show_on_slider);
    connect(detect_intv_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::update);
}

void MainWindow::open_widget(VideoProject* vid_proj) {
    if (video_widgets.size() < Singleton::get_instance()->FLOATING_WIDGET_MAX) {
        VideoWidget* widget_video = new VideoWidget(nullptr, true);
        widget_video->setMinimumSize(Constants::VIDEO_WGT_WIDTH * Constants::SIZE_MULTIPLIER,
                                     Constants::VIDEO_WGT_HEIGHT *Constants::SIZE_MULTIPLIER);
        widget_video->show();
        video_widgets.push_back(widget_video);

        widget_video->setAttribute(Qt::WA_DeleteOnClose);
        widget_video->frame_wgt->set_tool(ZOOM);
        widget_video->load_marked_video_state(vid_proj, vid_proj->get_video()->state);

        connect(widget_video, &VideoWidget::close_video_widget, this, &MainWindow::close_widget);
    } else {
        emit set_status_bar("No more video widgets allowed");
    }
}

void MainWindow::close_widget(VideoWidget *vid_wgt) {
    auto p = std::find(video_widgets.begin(), video_widgets.end(), vid_wgt);
    if (p != video_widgets.end()) {
        video_widgets.erase(p);
    }
}

void MainWindow::close_all_widgets() {
    for (auto widget : video_widgets) {
        delete widget;
    }
    video_widgets.clear();
}

void MainWindow::init_interval_menu() {
    QMenu* interval_menu = menuBar()->addMenu(tr("&Tag/Interval"));

    QAction* new_label_act = new QAction(tr("New tag &label..."));
    QAction* new_tag_act = new QAction(tr("New &tag"));
    QAction* remove_tag_act = new QAction(tr("&Delete tag"));
    QAction* tag_interval_act = new QAction(tr("&Tag interval"));
//    QAction* rm_interval_act = new QAction(tr("&Delete interval"), this);
    interval_act = new QAction(tr("&Interval"), this);

    interval_act->setCheckable(true);
    interval_act->setChecked(true);

    new_label_act->setIcon(QIcon("../ViAn/Icons/tag.png"));
    new_tag_act->setIcon(QIcon("../ViAn/Icons/tag_frame.png"));
    remove_tag_act->setIcon(QIcon("../ViAn/Icons/remove_frame.png"));
    tag_interval_act->setIcon(QIcon("../ViAn/Icons/create_interval.png"));

    new_label_act->setShortcut(tr("Ctrl+T"));
    new_tag_act->setShortcut(Qt::Key_T);
    remove_tag_act->setShortcut(Qt::Key_U);
    tag_interval_act->setShortcut(Qt::Key_K);
//    rm_interval_act->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Delete));

    new_label_act->setStatusTip(tr("Create new tag label"));
    new_tag_act->setStatusTip(tr("Tag the current frame"));
    remove_tag_act->setStatusTip(tr("Untag the current frame"));
    interval_act->setStatusTip(tr("Toggle interval on/off"));

    interval_menu->addAction(new_label_act);
    interval_menu->addAction(new_tag_act);
    interval_menu->addAction(remove_tag_act);
    interval_menu->addSeparator();
    interval_menu->addAction(tag_interval_act);
    interval_menu->addSeparator();
//    interval_menu->addAction(rm_interval_act);
    interval_menu->addAction(interval_act);

    connect(new_label_act, &QAction::triggered, video_wgt, &VideoWidget::new_tag_clicked);
    connect(new_tag_act, &QAction::triggered, video_wgt, &VideoWidget::tag_frame);
    connect(remove_tag_act, &QAction::triggered, video_wgt, &VideoWidget::remove_tag_frame);
    connect(tag_interval_act, &QAction::triggered, video_wgt, &VideoWidget::create_interval_clicked);
//    connect(rm_interval_act, &QAction::triggered, video_wgt, &VideoWidget::delete_interval);
    connect(interval_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::set_show_interval);
    connect(interval_act, &QAction::toggled, video_wgt->playback_slider, &AnalysisSlider::update);
}

/**
 * @brief MainWindow::init_tools_menu
 * Set up the tools menu
 */
void MainWindow::init_drawings_menu() {
    QMenu* tool_menu = menuBar()->addMenu(tr("&Drawings"));

    color_act = new QAction(tr("&Color..."), this);
    QAction* rectangle_act = new QAction(tr("&Rectangle"), this);
    QAction* circle_act = new QAction(tr("&Circle"), this);
    QAction* line_act = new QAction(tr("&Line"), this);
    QAction* arrow_act = new QAction(tr("&Arrow"), this);
    QAction* pen_act = new QAction(tr("&Pen"), this);
    QAction* text_act = new QAction(tr("&Text"), this);
    QAction* delete_drawing_act = new QAction(tr("&Delete drawing"), this);

    drawing_act = new QAction(tr("&Drawings"), this);
    drawing_act->setCheckable(true);
    drawing_act->setChecked(true);

    color_act->setIcon(QIcon("../ViAn/Icons/color.png"));
    rectangle_act->setIcon(QIcon("../ViAn/Icons/box.png"));
    circle_act->setIcon(QIcon("../ViAn/Icons/circle.png"));
    line_act->setIcon(QIcon("../ViAn/Icons/line.png"));
    arrow_act->setIcon(QIcon("../ViAn/Icons/arrow.png"));
    pen_act->setIcon(QIcon("../ViAn/Icons/pen.png"));
    text_act->setIcon(QIcon("../ViAn/Icons/text.png"));
    delete_drawing_act->setIcon(QIcon("../ViAn/Icons/clear.png"));

    tool_menu->addAction(color_act);
    QMenu* drawing_tools = tool_menu->addMenu(tr("&Shapes"));
    drawing_tools->addAction(rectangle_act);
    drawing_tools->addAction(circle_act);
    drawing_tools->addAction(arrow_act);
    drawing_tools->addAction(line_act);
    drawing_tools->addAction(pen_act);
    drawing_tools->addAction(text_act);

    tool_menu->addAction(delete_drawing_act);
    tool_menu->addSeparator();
    tool_menu->addAction(drawing_act);

    color_act->setStatusTip(tr("Color picker"));
    rectangle_act->setStatusTip(tr("Rectangle tool"));
    circle_act->setStatusTip(tr("Circle tool"));
    line_act->setStatusTip(tr("Line tool"));
    arrow_act->setStatusTip(tr("Arrow tool"));
    pen_act->setStatusTip(tr("Pen tool"));
    text_act->setStatusTip(tr("Text tool"));
    delete_drawing_act->setStatusTip(tr("Delete the current drawing"));
    drawing_act->setStatusTip(tr("Toggle drawings on/off"));

    rectangle_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    circle_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));
    arrow_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));
    line_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_4));
    pen_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_5));
    text_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_6));
    color_act->setShortcut(QKeySequence(Qt::ALT + Qt::Key_7));

    //Connect
    connect(rectangle_act, &QAction::triggered, draw_toolbar->rectangle_tool_act, &QAction::trigger);
    connect(circle_act, &QAction::triggered, draw_toolbar->circle_tool_act, &QAction::trigger);
    connect(line_act, &QAction::triggered, draw_toolbar->line_tool_act, &QAction::trigger);
    connect(arrow_act, &QAction::triggered, draw_toolbar->arrow_tool_act, &QAction::trigger);
    connect(pen_act, &QAction::triggered, draw_toolbar->pen_tool_act, &QAction::trigger);
    connect(text_act, &QAction::triggered, draw_toolbar->text_tool_act, &QAction::trigger);
    connect(delete_drawing_act, &QAction::triggered, this, &MainWindow::delete_current_drawing);
    connect(drawing_act, &QAction::toggled, video_wgt, &VideoWidget::set_show_overlay);
}

void MainWindow::init_export_menu() {
    QMenu* export_menu = menuBar()->addMenu(tr("E&xport"));

    QAction* export_act = new QAction(tr("E&xport interval..."), this);
    QAction* copy_frame_act = new QAction(tr("&Copy frame to clipboard"), this);
    QAction* gen_report_act = new QAction(tr("&Generate report"), this);

    export_act->setIcon(QIcon("../ViAn/Icons/folder_interval.png"));
    copy_frame_act->setIcon(QIcon("../ViAn/Icons/copy.png"));
    gen_report_act->setIcon(QIcon("../ViAn/Icons/report.png"));

    export_menu->addAction(export_act);
    export_menu->addAction(copy_frame_act);
    export_menu->addSeparator();
    export_menu->addAction(gen_report_act);

    export_act->setShortcut(tr("Shift+X"));
    copy_frame_act->setShortcut(QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_C));
    gen_report_act->setShortcuts(QKeySequence::Print);      //Ctrl + P

    export_act->setStatusTip(tr("Export all frames in an interval"));
    copy_frame_act->setStatusTip("Copy the current frame to the clipboard");
    gen_report_act->setStatusTip(tr("Generate report"));

    connect(export_act, &QAction::triggered, this, &MainWindow::export_images);
    connect(copy_frame_act, &QAction::triggered, video_wgt->frame_wgt, &FrameWidget::copy_to_clipboard);
    connect(gen_report_act, &QAction::triggered, bookmark_wgt, &BookmarkWidget::generate_report);
    connect(bookmark_wgt, &BookmarkWidget::play_video, video_wgt, &VideoWidget::play_btn_toggled);
}

/**
 * @brief MainWindow::init_help_menu
 * Set up the help menu
 */
void MainWindow::init_help_menu() {
    QMenu* help_menu = menuBar()->addMenu(tr("&Help"));
    QAction* help_act = new QAction(tr("&Open manual"), this);
    help_act->setIcon(QIcon("../ViAn/Icons/question.png"));
    help_menu->addAction(help_act);
    help_act->setShortcut(QKeySequence(Qt::Key_F1));
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

void MainWindow::init_rp_dialog() {
    rp_dialog = new RecentProjectDialog(this);
    rp_dialog->setAttribute(Qt::WA_DeleteOnClose);

    // Create a new project if user presses "new project" or if dialog is rejected
    connect(rp_dialog, &RecentProjectDialog::new_project, project_wgt, &ProjectWidget::new_project);
    connect(rp_dialog, &RecentProjectDialog::open_project, project_wgt, &ProjectWidget::open_project);
    connect(rp_dialog, &RecentProjectDialog::open_project_from_file, project_wgt, &ProjectWidget::open_project);
    connect(rp_dialog, &RecentProjectDialog::remove_project, project_wgt, &ProjectWidget::remove_project);
    connect(rp_dialog, &RecentProjectDialog::exit, this, &QWidget::close);
}

void MainWindow::open_rp_dialog() {
    init_rp_dialog();
    rp_dialog->exec();
}

/**
 * @brief MainWindow::cont_bri
 * Contrast/brightness triggered.
 * Creates a dialog for choosing contrast and brightness values.
 */
void MainWindow::cont_bri() {
    if (video_wgt->get_current_video_project() == nullptr) return;
    emit set_status_bar("Opening color correction settings");
    manipulator_dock->show();
}

void MainWindow::export_images() {
    if (!video_wgt || !video_wgt->playback_slider) return;
    std::pair<int, int> interval = video_wgt->playback_slider->get_valid_interval();
    VideoProject* vid_proj = video_wgt->get_current_video_project();
    if (vid_proj == nullptr){
        emit set_status_bar("A video needs to be selected");
        return;
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
    SettingsDialog* dialog = new SettingsDialog(this);
    int status = dialog->exec();
    if (status) {
        Singleton* s = Singleton::get_instance();
        if (project_wgt) project_wgt->setIconSize(s->PROJ_THUMBNAIL_SIZE);
        if (video_wgt && video_wgt->playback_slider) video_wgt->playback_slider->setPageStep(s->PAGE_STEP);
    }
}

void MainWindow::open_project_dialog(){
    QString project_path = QFileDialog().getOpenFileName(
                this,
                tr("Open project"),
                Constants::DEFAULT_PATH,
                "*.vian");
    project_wgt->open_project(project_path);
}

void MainWindow::open_project_folder() {
    if (!project_wgt->m_proj) return;
    QString dir = project_wgt->m_proj->get_dir();
    QDesktopServices::openUrl(QUrl("file:///"+dir, QUrl::TolerantMode));
}

void MainWindow::view_paths() {
    ViewPathDialog* path_dialog = new ViewPathDialog(project_wgt->video_list, this);

    int status = path_dialog->exec();

    if (status == path_dialog->Accepted) {
        project_wgt->update_videoitems();
        if(video_wgt->get_current_video_project()) {
            video_wgt->get_current_video_project()->set_current(false);
            project_wgt->currentItemChanged(project_wgt->currentItem(), project_wgt->currentItem());
        }
    }
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

void MainWindow::show_bookmark_dock(bool show) {
    if (show) {
        bookmark_dock->show();
    }
}
