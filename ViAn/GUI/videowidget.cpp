#include "videowidget.h"

#include "Analysis/analysissettings.h"
#include "doubleclickbutton.h"
#include "framewidget.h"
#include "GUI/Analysis/analysisslider.h"
#include "GUI/Analysis/tagdialog.h"
#include "GUI/Bookmark/bookmarkdialog.h"
#include "GUI/drawscrollarea.h"
#include "GUI/TreeItems/treeitem.h"
#include "Project/Analysis/analysisinterval.h"
#include "Project/Analysis/interval.h"
#include "Project/Analysis/tag.h"
#include "Project/Analysis/tagframe.h"
#include "Project/videoproject.h"
#include "Video/videocontroller.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QShortcut>
#include <QSlider>
#include <QThread>

#include "pathdialog.h"


VideoWidget::VideoWidget(QWidget *parent, bool floating) : QWidget(parent), scroll_area(new DrawScrollArea) {
    m_floating = floating;
    // Init video controller
    v_controller = new VideoController(&frame_index, &is_playing, &new_frame,
                                       &video_width, &video_height, &new_video, &new_frame_video, &video_loaded, &v_sync,
                                       &player_con, &player_lock, &m_video_path,
                                       &m_speed_step, &m_abort_playback);

    //Setup playback area
    vertical_layout = new QVBoxLayout;
    frame_wgt = new FrameWidget();
    frame_wgt->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QHBoxLayout* hlout = new QHBoxLayout;
    hlout->setAlignment(Qt::AlignCenter);
    hlout->addWidget(frame_wgt);
    hlout->setContentsMargins(0,0,0,0);
    scroll_area->setLayout(hlout);
    scroll_area->setFrameStyle(0);
    v_bar = scroll_area->verticalScrollBar();
    h_bar = scroll_area->horizontalScrollBar();
    v_bar->setSingleStep(1);
    v_bar->hide();
    h_bar->hide();
    vertical_layout->addWidget(scroll_area);

    // End playback setup

    init_control_buttons();
    init_playback_slider();
    setLayout(vertical_layout);

    qRegisterMetaType<cv::Mat>("cv::Mat");

    connect(scroll_area, &DrawScrollArea::new_size, frame_wgt, &FrameWidget::set_scroll_area_size);
    connect(this, &VideoWidget::set_detections_on_frame, frame_wgt, &FrameWidget::set_detections_on_frame);

    init_video_controller();

    v_controller->start();
    init_frame_processor();
}

VideoWidget::~VideoWidget(){
    play_btn_toggled(false);
    m_abort_processor.store(true);
    v_sync.con_var.notify_all();
    if (processing_thread->isRunning()) {
        processing_thread->deleteLater();
        processing_thread->exit();
        if (!processing_thread->wait(Constants::FIVE_SEC)) {
            processing_thread->terminate();
            processing_thread->wait();
        }
    }

    if (v_controller->isRunning()) {
        // Tell controller thread to exit and wait for it to finish
        m_abort_playback.store(true);
        player_con.notify_all();
        v_controller->exit();
        if (!v_controller->wait(Constants::FIVE_SEC)) {
            // Controller did not finish in time. Force shutdown
            v_controller->terminate();
            v_controller->wait();
        }
    }
    delete f_processor;
    delete processing_thread;

    delete v_controller;
    delete frame_wgt;
}

void VideoWidget::closeEvent(QCloseEvent *event) {
    event->accept();
    emit close_video_widget(this);
}

VideoProject *VideoWidget::get_current_video_project(){
    return m_vid_proj;
}

/**
 * @brief VideoWidget::get_current_video_length
 * @return The length of the current video in frames
 */
int VideoWidget::get_current_video_length(){
    return m_frame_length;
}

void VideoWidget::quick_analysis(AnalysisSettings * settings) {
    if(m_interval.first != -1 && m_interval.second != -1 && (m_interval.first < m_interval.second)) {
        settings->set_interval(m_interval);
    }
    emit start_analysis(m_vid_proj, settings);
}

/**
 * @brief VideoWidget::init_btn_layout
 * Set up the button layouts
 */
void VideoWidget::init_control_buttons() {
    init_layouts();
    set_btn_icons();
    set_btn_tool_tip();
    set_btn_size();
    set_btn_tab_order();
    set_btn_shortcuts();
    init_speed_slider();
    add_btns_to_layouts();
    connect_btns();
}

/**
 * @brief VideoWidget::init_btn_layout
 * Set up the button layouts
 */
void VideoWidget::init_layouts() {
    control_row = new QHBoxLayout;     // Container for all button areas
    video_btns = new QHBoxLayout;      // Play, pause etc
    analysis_btns = new QHBoxLayout;   // Buttons for starting analysis and jumping between pois
    other_btns = new QHBoxLayout;      // Bookmark, tag
    zoom_btns = new QHBoxLayout;       // Zoom buttons
    interval_btns = new QHBoxLayout;   // Interval buttons    
    control_row->setAlignment(Qt::AlignLeft);
    video_btns->setSpacing(5);
    analysis_btns->setSpacing(5);
    other_btns->setSpacing(5);
    zoom_btns->setSpacing(5);
    interval_btns->setSpacing(5);
    control_row->setSpacing(15);

    speed_slider_layout = new QGridLayout;
}

/**
 * @brief VideoWidget::init_video_controller
 * Performs the initial setup that connects the video controller with the rest of the program.
 */
void VideoWidget::init_video_controller(){
    // Video data
    connect(v_controller, &VideoController::capture_failed, this, &VideoWidget::capture_failed);
    connect(v_controller, &VideoController::video_info, this, &VideoWidget::on_video_info);
    connect(v_controller, &VideoController::display_index, this, &VideoWidget::display_index_slot);
    connect(v_controller, &VideoController::playback_stopped, this, &VideoWidget::on_playback_stopped);
    connect(v_controller, &VideoController::finished, v_controller, &VideoController::deleteLater);
}

/**
 * @brief VideoWidget::init_frame_processor
 * Creates a new FrameProcessor instance which is then moved to a new thread.
 */
void VideoWidget::init_frame_processor() {
    f_processor = new FrameProcessor(&new_frame, &settings_changed, &z_settings,
                                     &video_width, &video_height, &new_frame_video,
                                     &m_settings, &v_sync, &frame_index, &o_settings,
                                     &overlay_changed, &m_abort_processor, &m_frame_changing);

    try {
        processing_thread = new QThread(this);

        f_processor->moveToThread(processing_thread);
        connect(processing_thread, &QThread::started, f_processor, &FrameProcessor::check_events);
        connect(f_processor, &FrameProcessor::done_processing, frame_wgt, &FrameWidget::on_new_image);
        connect(f_processor, &FrameProcessor::zoom_preview, this, &VideoWidget::zoom_preview);
        connect(f_processor, &FrameProcessor::new_frame_size, this, &VideoWidget::set_frame_size);

        connect(frame_wgt, &FrameWidget::zoom_points, this, &VideoWidget::set_zoom_area);
        connect(scroll_area, &DrawScrollArea::new_size, this, &VideoWidget::set_draw_area_size);
        connect(frame_wgt, &FrameWidget::moved_xy, this, &VideoWidget::pan);
        connect(frame_wgt, &FrameWidget::move_viewport_center, this, &VideoWidget::center);
        connect(frame_wgt, &FrameWidget::mouse_double_click, this, &VideoWidget::mouse_double_clicked);
        connect(frame_wgt, &FrameWidget::mouse_pressed, this, &VideoWidget::mouse_pressed);
        connect(frame_wgt, &FrameWidget::mouse_released, this, &VideoWidget::mouse_released);
        connect(frame_wgt, &FrameWidget::mouse_moved, this, &VideoWidget::mouse_moved);
        connect(frame_wgt, &FrameWidget::mouse_scroll, this, &VideoWidget::mouse_scroll);
        connect(frame_wgt, &FrameWidget::process_frame, this, &VideoWidget::process_frame);
        connect(frame_wgt, &FrameWidget::send_tool, this, &VideoWidget::set_tool);
        connect(frame_wgt, &FrameWidget::send_tool_text, this, &VideoWidget::set_tool_text);
        connect(frame_wgt, &FrameWidget::send_color, this, &VideoWidget::set_color);

        connect(f_processor, &FrameProcessor::set_scale_factor, frame_wgt, &FrameWidget::set_scale_factor);
        connect(f_processor, &FrameProcessor::set_scale_factor, this, &VideoWidget::set_scale_factor);
        connect(f_processor, &FrameProcessor::set_anchor, frame_wgt, &FrameWidget::set_anchor);
        connect(f_processor, &FrameProcessor::set_rotation, frame_wgt, &FrameWidget::set_rotation);
        connect(f_processor, &FrameProcessor::set_flip, frame_wgt, &FrameWidget::set_flip);
        connect(f_processor, &FrameProcessor::set_play_btn, this->play_btn, &QPushButton::toggle);
        connect(f_processor, &FrameProcessor::set_zoom_state, this, &VideoWidget::set_zoom_state);
        connect(f_processor, &FrameProcessor::set_bri_cont, this, &VideoWidget::set_brightness_contrast);

        processing_thread->start();

    } catch (const std::bad_alloc& e) {
        qWarning() << "Failed to open new thread";
        close();
    }
}

/**
 * @brief VideoWidget::set_icons
 * Set icons on all buttons
 */
void VideoWidget::set_btn_icons() {
    play_btn = new QPushButton(QIcon(":/Icons/play.png"), "", this);
    stop_btn = new QPushButton(QIcon(":/Icons/stop.png"), "", this);
    next_frame_btn = new QPushButton(QIcon(":/Icons/next_frame.png"), "", this);
    prev_frame_btn = new QPushButton(QIcon(":/Icons/prev_frame.png"), "", this);
    next_poi_btn = new QPushButton(QIcon(":/Icons/next.png"), "", this);
    prev_poi_btn = new DoubleClickButton(this);
    prev_poi_btn->setIcon(QIcon(":/Icons/prev.png"));
    bookmark_btn = new QPushButton(QIcon(":/Icons/bookmark.png"), "", this);
    export_frame_btn = new QPushButton(QIcon(":/Icons/export.png"), "", this);

    analysis_play_btn = new QPushButton(QIcon(":/Icons/play+check.png"), "", this);
    tag_btn = new QPushButton(QIcon(":/Icons/tag_frame.png"), "", this);
    new_label_btn = new QPushButton(QIcon(":/Icons/tag.png"), "", this);
    set_start_interval_btn = new QPushButton(QIcon(":/Icons/start_interval.png"), "", this);
    set_end_interval_btn = new QPushButton(QIcon(":/Icons/end_interval.png"), "", this);
    create_interval_btn = new QPushButton(QIcon(":/Icons/create_interval.png"), "", this);
    interval_to_edit_btn = new QPushButton(QIcon(":/Icons/add_interval.png"), "", this);
    loop_btn = new QPushButton(QIcon(":/Icons/loop.png"), "", this);

    interpolate_check = new QCheckBox("Interpolate", this);
    fit_btn = new QPushButton(QIcon(":/Icons/resize.png"), "", this);
    original_size_btn = new QPushButton(QIcon(":/Icons/original_size.png"), "", this);


    zoom_label = new QLineEdit("100%", this);
    zoom_label->setMinimumWidth(40);
    zoom_label->setMaximumWidth(60);
    zoom_label->setEnabled(false);
    connect(zoom_label, &QLineEdit::editingFinished, this, &VideoWidget::zoom_label_finished);
    video_label = new QLabel("", this);
    fps_label = new QLabel("0fps", this);
    size_label = new QLabel("(0x0)", this);
    rotation_label = new QLabel("0°", this);

    play_btn->setCheckable(true);
    analysis_play_btn->setCheckable(true);
    loop_btn->setCheckable(true);
}

/**
 * @brief VideoWidget::set_tool_tip
 * Set tooltip on all buttons
 */
void VideoWidget::set_btn_tool_tip() {
    play_btn->setToolTip(tr("Play: Space"));
    stop_btn->setToolTip(tr("Stop: S"));
    next_frame_btn->setToolTip(tr("Next frame: Right"));
    prev_frame_btn->setToolTip(tr("Previous frame: Left"));

    next_poi_btn->setToolTip(tr("Next POI: Ctrl + Right"));
    prev_poi_btn->setToolTip(tr("Previous POI: Ctrl + Left"));
    analysis_play_btn->setToolTip(tr("Play only the POIs"));

    bookmark_btn->setToolTip(tr("Bookmark current frame: Ctrl + B"));
    export_frame_btn->setToolTip("Export current frame: X");
    tag_btn->setToolTip(tr("Tag the current frame: T"));
    new_label_btn->setToolTip(tr("Create a new label: Ctrl + T"));
    set_start_interval_btn->setToolTip("Set left interval point: I");
    set_end_interval_btn->setToolTip("Set right interval point: O");
    create_interval_btn->setToolTip("Save the current marked interval: K");
    interval_to_edit_btn->setToolTip(tr("Add the interval directly to the video edit widget: P"));
    loop_btn->setToolTip("Loop the marked interval");

    fit_btn->setToolTip(tr("Scale video to screen: Ctrl + F"));
    original_size_btn->setToolTip(tr("Reset zoom: Ctrl + H"));
    zoom_label->setToolTip("The zoom factor of the video: Z");
    interpolate_check->setToolTip("Toggle between bicubic and nearest neighbor interpolation: N");

    video_label->setToolTip("Name of the current video");
    fps_label->setToolTip("The frame rate of the video");
    size_label->setToolTip("The size of  the video");
    rotation_label->setToolTip("The current rotation of the video");
}

/**
 * @brief VideoWidget::set_btn_size
 * Set the size on all buttons
 */
void VideoWidget::set_btn_size() {
    btns.push_back(play_btn);
    btns.push_back(stop_btn);
    btns.push_back(next_frame_btn);
    btns.push_back(prev_frame_btn);
    btns.push_back(bookmark_btn);
    btns.push_back(export_frame_btn);
    btns.push_back(new_label_btn);
    btns.push_back(set_start_interval_btn);
    btns.push_back(set_end_interval_btn);
    btns.push_back(create_interval_btn);
    btns.push_back(interval_to_edit_btn);
    btns.push_back(fit_btn);
    btns.push_back(original_size_btn);
    for (QPushButton* btn : btns) {
        btn->setFixedSize(BTN_SIZE);
        btn->setEnabled(false);
    }
    next_poi_btn->setFixedSize(BTN_SIZE);
    prev_poi_btn->setFixedSize(BTN_SIZE);
    tag_btn->setFixedSize(BTN_SIZE);
    analysis_play_btn->setFixedSize(BTN_SIZE);
    loop_btn->setFixedSize(BTN_SIZE);
    enable_poi_btns(false,false);
    tag_btn->setEnabled(false);
    interpolate_check->setEnabled(false);
}

/**
 * @brief VideoWidget::set_tab_order
 * Set the tab order for the buttons
 */
void VideoWidget::set_btn_tab_order() {
    setTabOrder(prev_frame_btn, play_btn);
    setTabOrder(play_btn, next_frame_btn);
    setTabOrder(next_frame_btn, stop_btn);
    setTabOrder(stop_btn, prev_poi_btn);
    setTabOrder(next_poi_btn, analysis_play_btn);
    setTabOrder(analysis_play_btn, bookmark_btn);
    setTabOrder(bookmark_btn, export_frame_btn);
    setTabOrder(export_frame_btn, tag_btn);
    setTabOrder(tag_btn, new_label_btn);
    setTabOrder(new_label_btn, set_start_interval_btn);
    setTabOrder(set_start_interval_btn, set_end_interval_btn);
    setTabOrder(set_end_interval_btn, create_interval_btn);
    setTabOrder(create_interval_btn, interval_to_edit_btn);
    setTabOrder(interval_to_edit_btn, loop_btn);
    setTabOrder(loop_btn, fit_btn);
    setTabOrder(fit_btn, original_size_btn);
    setTabOrder(original_size_btn, interpolate_check);
}

/**
 * @brief VideoWidget::set_btn_shortcuts
 * Set shortcuts to the buttons
 */
void VideoWidget::set_btn_shortcuts() {
    play_btn->setShortcut(QKeySequence(Qt::Key_Space));
    play_btn->setFocusPolicy(Qt::NoFocus);
    stop_btn->setShortcut(QKeySequence(Qt::Key_S));
    next_frame_btn->setShortcut(Qt::Key_Right);
    prev_frame_btn->setShortcut(Qt::Key_Left);
    next_poi_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    prev_poi_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    // Tag, zoom and interval shortcuts are in the menus

    zoom_edit_sc = new QShortcut(QKeySequence(Qt::Key_Z), this);
    interpol_sc = new QShortcut(QKeySequence(Qt::Key_N), this);
    video_start_sc = new QShortcut(QKeySequence(Qt::Key_Home), this);
    video_end_sc = new QShortcut(QKeySequence(Qt::Key_End), this);
    page_step_front_sc = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Right), this);
    page_step_back_sc = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Left), this);
    delete_sc = new QShortcut(QKeySequence::Delete, this);
    delete_sc->setContext(Qt::WidgetWithChildrenShortcut);

    //connect
    connect(zoom_edit_sc, &QShortcut::activated, this, &VideoWidget::zoom_label_focus);
    connect(interpol_sc, &QShortcut::activated, interpolate_check, &QCheckBox::toggle);
    connect(video_start_sc, &QShortcut::activated, this, &VideoWidget::set_video_start);
    connect(video_end_sc, &QShortcut::activated, this, &VideoWidget::set_video_end);
    connect(page_step_front_sc, &QShortcut::activated, this, &VideoWidget::page_step_front);
    connect(page_step_back_sc, &QShortcut::activated, this, &VideoWidget::page_step_back);
    connect(delete_sc, &QShortcut::activated, this, &VideoWidget::delete_sc_activated);
}

/**
 * @brief VideoWidget::init_speed_slider
 * Create and add speed adjustment slider
 */
void VideoWidget::init_speed_slider() {
    speed_slider = new QSlider(Qt::Horizontal, this);
    speed_slider->setRange(-4,4);
    speed_slider->setMaximumWidth(120);
    speed_slider->setPageStep(1);
    speed_slider->setTickPosition(QSlider::TicksBelow);
    speed_slider->setEnabled(false);
    speed_slider->setToolTip(tr("Adjust playback speed: * & /"));
    QLabel *label1 = new QLabel("1/8x", this);
    QLabel *label2 = new QLabel("1x", this);
    QLabel *label3 = new QLabel("8x", this);
    QFont f("Helvetica", 6, QFont::Normal);
    label1->setFont(f);
    label2->setFont(f);
    label3->setFont(f);
    //layout->addWidget(widget, int row, int column, int rowSpan, int columnSpan)
    speed_slider_layout->addWidget(speed_slider, 0, 0, 1, 5);
    speed_slider_layout->addWidget(label1, 1, 0, 1, 1);
    speed_slider_layout->addWidget(label2, 1, 2, 1, 1);
    speed_slider_layout->addWidget(label3, 1, 4, 1, 1);

    speed_slider_up = new QShortcut(QKeySequence(Qt::Key_Asterisk), this);
    speed_slider_down = new QShortcut(QKeySequence(Qt::Key_Slash), this);

    connect(speed_slider_up, &QShortcut::activated, this, &VideoWidget::speed_up_activate);
    connect(speed_slider_down, &QShortcut::activated, this, &VideoWidget::speed_down_activate);
    connect(speed_slider, &QSlider::valueChanged, this, &VideoWidget::update_playback_speed);
}

/**
 * @brief VideoWidget::add_btns_to_layouts
 * Add the buttons to the layouts
 */
void VideoWidget::add_btns_to_layouts() {
    video_btns->addWidget(prev_frame_btn);
    video_btns->addWidget(play_btn);
    video_btns->addWidget(next_frame_btn);
    video_btns->addWidget(stop_btn);
    video_btns->addLayout(speed_slider_layout);

    control_row->addLayout(video_btns);

    analysis_btns->addWidget(prev_poi_btn);
    analysis_btns->addWidget(next_poi_btn);
    analysis_btns->addWidget(analysis_play_btn);

    control_row->addLayout(analysis_btns);

    other_btns->addWidget(bookmark_btn);
    other_btns->addWidget(export_frame_btn);
    other_btns->addWidget(tag_btn);
    other_btns->addWidget(new_label_btn);
    other_btns->addWidget(set_start_interval_btn);
    other_btns->addWidget(set_end_interval_btn);
    other_btns->addWidget(create_interval_btn);
    other_btns->addWidget(interval_to_edit_btn);
    other_btns->addWidget(loop_btn);

    control_row->addLayout(other_btns);

    zoom_btns->addWidget(fit_btn);
    zoom_btns->addWidget(original_size_btn);
    zoom_btns->addWidget(zoom_label);
    zoom_btns->addWidget(interpolate_check);

    control_row->addLayout(zoom_btns);

    interval_btns->addWidget(video_label);
    interval_btns->addWidget(fps_label);
    interval_btns->addWidget(size_label);
    interval_btns->addWidget(rotation_label);

    control_row->addLayout(interval_btns);

    vertical_layout->addLayout(control_row);
}

/**
 * @brief VideoWidget::connect_btns
 * Connect all control buttons with signals and slots
 */
void VideoWidget::connect_btns() {
    // Playback
    connect(play_btn, &QPushButton::toggled, this, &VideoWidget::play_btn_toggled);
    connect(stop_btn, &QPushButton::clicked, this, &VideoWidget::stop_btn_clicked);
    connect(next_frame_btn, &QPushButton::clicked, this, &VideoWidget::next_frame_clicked);
    connect(prev_frame_btn, &QPushButton::clicked, this, &VideoWidget::prev_frame_clicked);

    // Analysis
    connect(analysis_play_btn, &QPushButton::toggled, this, &VideoWidget::analysis_play_btn_toggled);
    connect(next_poi_btn, &QPushButton::clicked, this, &VideoWidget::next_poi_btn_clicked);
    connect(prev_poi_btn, &DoubleClickButton::clicked, this, &VideoWidget::prev_poi_btn_clicked);
    connect(prev_poi_btn, &DoubleClickButton::double_clicked, this, &VideoWidget::prev_poi_btn_clicked);

    // Tag
    connect(tag_btn, &QPushButton::clicked, this, &VideoWidget::tag_frame);
    connect(new_label_btn, &QPushButton::clicked, this, &VideoWidget::new_tag_clicked);

    // Zoom
    connect(frame_wgt, &FrameWidget::trigger_zoom_out, this, &VideoWidget::on_step_zoom);
    connect(fit_btn, &QPushButton::clicked, this, &VideoWidget::on_fit_screen);
    connect(original_size_btn, &QPushButton::clicked, this, &VideoWidget::on_original_size);
    connect(interpolate_check, &QCheckBox::toggled, this, &VideoWidget::on_interpolate_toggled);

    // Other
    connect(bookmark_btn, &QPushButton::clicked, this, &VideoWidget::on_bookmark_clicked);
    connect(export_frame_btn, &QPushButton::clicked, this, &VideoWidget::on_export_frame);

    // Interval
    connect(set_start_interval_btn, &QPushButton::clicked, this, &VideoWidget::set_interval_start_clicked);
    connect(set_end_interval_btn, &QPushButton::clicked, this, &VideoWidget::set_interval_end_clicked);
    connect(create_interval_btn, &QPushButton::clicked, this, &VideoWidget::create_interval_clicked);
    connect(interval_to_edit_btn, &QPushButton::clicked, this, &VideoWidget::add_interval_clicked);
    connect(loop_btn, &QPushButton::toggled, this, &VideoWidget::loop_interval_toggled);
}

/**
 * @brief Adds the video slider and playback timestamps
 */
void VideoWidget::init_playback_slider() {
    QHBoxLayout* progress_area = new QHBoxLayout();
    current_time = new QLabel("--:--");
    total_time = new QLabel("--:--");
    max_frames = new QLabel("0", this);
    frame_line_edit = new QLineEdit("0", this);

    frame_line_edit->setFixedWidth(50);
    frame_line_edit->setToolTip("Jump to frame: F");

    frame_edit_sc = new QShortcut(QKeySequence(Qt::Key_F), this);
    connect(frame_edit_sc, &QShortcut::activated, this, &VideoWidget::frame_label_focus);

    playback_slider = new AnalysisSlider(Qt::Horizontal, this);

    frame_line_edit->setEnabled(false);
    playback_slider->setEnabled(false);
    progress_area->addWidget(current_time);
    progress_area->addWidget(playback_slider);
    progress_area->addWidget(total_time);
    progress_area->addWidget(frame_line_edit);
    progress_area->addWidget(max_frames);
    vertical_layout->addLayout(progress_area);

    connect(playback_slider, &QSlider::sliderPressed, this, &VideoWidget::on_playback_slider_pressed);
    connect(playback_slider, &QSlider::sliderReleased, this, &VideoWidget::on_playback_slider_released);
    connect(playback_slider, &QSlider::valueChanged, this, &VideoWidget::on_playback_slider_value_changed);
    connect(playback_slider, &QSlider::sliderMoved, this, &VideoWidget::on_playback_slider_moved);

    connect(frame_line_edit, &QLineEdit::editingFinished, this, &VideoWidget::frame_line_edit_finished);
}

void VideoWidget::stop_btn_clicked() {
    set_status_bar("Stop");
    {
        std::lock_guard<std::mutex> lk(player_lock);
        frame_index.store(0);
        is_playing.store(false);
    }
    play_btn->setChecked(false);

    on_new_frame();
}

/**
 * @brief VideoWidget::next_frame_clicked
 */
void VideoWidget::next_frame_clicked() {
    if (analysis_only) {
        if (playback_slider->is_poi_end(frame_index.load())) {
            next_poi_btn_clicked();
            return;
        }
    }
    if (playback_slider->value() + 1 < m_frame_length) {
        set_status_bar("Stepping to the next frame");
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            frame_index.store(playback_slider->value() +1);
        }
        on_new_frame();
    } else {
        set_status_bar("Already at the last frame");
    }
}

/**
 * @brief VideoWidget::prev_frame_clicked
 */
void VideoWidget::prev_frame_clicked() {
    int current_frame = frame_index.load();
    if (analysis_only) {
        if (playback_slider->is_poi_start(current_frame)) {
            int new_frame = playback_slider->get_prev_poi_end(current_frame);
            {
                std::lock_guard<std::mutex> p_lock(player_lock);
                frame_index.store(new_frame);
            }
            on_new_frame();
            return;
        }
    }
    if (playback_slider->value() - 1 > -1) {
        set_status_bar("Stepping to the previous frame");
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            frame_index.store(playback_slider->value() - 1);
        }
        on_new_frame();
    } else {
        set_status_bar("Already at the first frame");
    }
}

/**
 * @brief VideoWidget::on_interpolate_toggled
 * Toggles between bicubic (true) or nearest neighbor interpolation (false)
 * @param checked
 */
void VideoWidget::on_interpolate_toggled(bool checked) {
    int method = checked ? cv::INTER_CUBIC : cv::INTER_NEAREST;
    set_interpolation_method(method);
}

/**
 * @brief converts the time in ms to a formatted string
 * @param time in ms
 * @return correctly formatted QString
 */
QString VideoWidget::convert_time(int time) {
    QTime q_time((time/3600)%60, (time/60)%60, time%60);
    QString format = "mm:ss";
    if (time > 3600)
        format = "hh:mm:ss";
    return q_time.toString(format);
}

/**
 * @brief sets the current playback time to label
 * @param current playback time in ms
 */
void VideoWidget::set_current_time(int time) {
    current_time->setText(convert_time(time));
}

/**
 * @brief sets the total playback time to label
 * @param total time in ms
 */
void VideoWidget::set_total_time(int time) {
    total_time->setText(convert_time(time));
}

/**
 * @brief VideoWidget::set_scale_factor
 * @param scale_factor
 */
void VideoWidget::set_scale_factor(double scale_factor) {
    m_scale_factor = scale_factor;
    zoom_label->setText(QString::number((static_cast<int>((10000*m_scale_factor)))/100.) +"%");
}

/**
 * @brief VideoWidget::set_zoom_state
 * Stores the current zoom modifications and rotation made to the video
 */
void VideoWidget::set_zoom_state(QPoint center, double scale, int angle, bool flip_h, bool flip_v) {
    if (!m_vid_proj) return;
    if (!m_floating) {
        if (proj_tree_item == VIDEO_ITEM) {
            m_vid_proj->state.center = center;
            m_vid_proj->state.scale_factor = scale;
            m_vid_proj->state.rotation = angle;
            m_vid_proj->state.flip_h = flip_h;
            m_vid_proj->state.flip_v = flip_v;
        }
        if (m_tag && m_vid_proj->get_video()->get_sequence_type() == TAG_SEQUENCE) {
            TagFrame* t_frame = m_tag->tag_map.at(frame_index.load());
            t_frame->set_center(center);
            t_frame->set_scale_rot(scale, angle, flip_h, flip_v);
        }
        Video* video = m_vid_proj->get_video();
        video->state.center = center;
        video->state.scale_factor = scale;
        video->state.rotation = angle;
        video->state.flip_h = flip_h;
        video->state.flip_v = flip_v;

        if (angle == 90 || angle == 270) {
            size_label->setText("(" + QString::number(m_video_height) + "x" + QString::number(m_video_width) + ")");
        } else {
            size_label->setText("(" + QString::number(m_video_width) + "x" + QString::number(m_video_height) + ")");
        }
        rotation_label->setText(QString::number(angle) + "°");
    }
}

/**
 * @brief VideoWidget::on_bookmark_clicked
 */
void VideoWidget::on_bookmark_clicked() {
    if (frame_is_clean) return;
    BookmarkDialog dialog;
    bool ok = dialog.exec();
    bmark_description = dialog.textValue();
    if (!ok) return;
    quick_bookmark();
}

void VideoWidget::quick_bookmark() {
    if (frame_is_clean) return;
    cv::Mat bookmark_frame = frame_wgt->get_modified_frame();
    cv::Mat org_frame = frame_wgt->get_org_frame();
    QString time = current_time->text();
    emit new_bookmark(m_vid_proj, m_vid_proj->get_video()->state, bookmark_frame, org_frame, time, bmark_description);
    bmark_description = "";
}

/**
 * @brief VideoWidget::set_interval_start_clicked
 * Sets the start point of the frame interval
 */
void VideoWidget::set_interval_start_clicked() {
    if (!playback_slider->is_show_interval()) return;
    int frame = playback_slider->set_interval_first();
    m_interval.first = frame;
    m_vid_proj->m_interval.first = frame;
    set_status_bar("Frame interval updated: " +
                   QString().number(m_interval.first) + "-" + QString().number(m_interval.second));
    playback_slider->update();
}

/**
 * @brief VideoWidget::set_interval_end_clicked
 * Sets the end of the frame interval
*/
void VideoWidget::set_interval_end_clicked() {
    if (!playback_slider->is_show_interval()) return;
    int frame = playback_slider->set_interval_second();
    m_interval.second = frame;
    m_vid_proj->m_interval.second = frame;
    set_status_bar("Frame interval updated: " +
                   QString().number(m_interval.first) + "-" + QString().number(m_interval.second));
    playback_slider->update();
}

/**
 * @brief VideoWidget::create_interval_clicked
 * Slot function for create interval button
 * Create a new interval if needed and then creates interval area
 * from the interval on the slider
 */
void VideoWidget::create_interval_clicked() {
    if (!playback_slider->is_show_interval()) return;
    int first = m_interval.first;
    int second = m_interval.second;
    if (first == -1 || second <= 0 || second < first) {
        emit set_status_bar("Select an interval");
        return;
    }
    if (m_current_interval == nullptr) {
        TagDialog* interval_dialog = new TagDialog("Interval");
        interval_dialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(interval_dialog, &TagDialog::tag_name, this, &VideoWidget::new_interval);
        int result = interval_dialog->exec();
        if (result != TagDialog::Accepted) return;
    }
    m_current_interval->add_area(first, second);
    emit set_interval_area(first);
    emit set_status_bar("Interval: " + QString::number(first) + " - " +
                        QString::number(second) + " added");
    playback_slider->update();
}

void VideoWidget::add_interval_clicked() {
    if (!playback_slider->is_show_interval()) return;
    int first = m_interval.first;
    int second = m_interval.second;
    if (first == -1 || second <= 0 || second < first) {
        emit set_status_bar("Select an interval");
        return;
    }
    emit interval_to_edit(first, second, m_vid_proj);
}

void VideoWidget::new_interval(QString name) {
    Interval* interval = new Interval(name);
    emit add_interval(m_vid_proj, interval);
}

void VideoWidget::loop_interval_toggled(bool value) {
    loop = value;
}

void VideoWidget::set_interval(std::pair<int, int> interval) {
    m_interval = interval;
    playback_slider->set_interval(interval);
}

/**
 * @brief VideoWidget::delete_interval
 */
void VideoWidget::delete_interval() {
    m_interval.first = -1;
    m_interval.second = -1;
    if (m_vid_proj) m_vid_proj->m_interval = m_interval;
    playback_slider->clear_interval();
}

/**
 * @brief VideoWidget::play_btn_toggled
 * Slot function called when the play/pause button is toggled.
 * Status = true indicates that the button has been put in its play state
 * @param status
 */
void VideoWidget::play_btn_toggled(bool status) {
    {
        std::lock_guard<std::mutex> lk(player_lock);
        is_playing.store(status);
    }
    if (status) {
        emit set_zoom_tool();
        set_status_bar("Play");
        play_btn->setIcon(QIcon(":/Icons/pause.png"));
    } else {
        if (analysis_only) {
            int new_frame = playback_slider->get_closest_poi(playback_slider->value());
            {
                std::lock_guard<std::mutex> lk(player_lock);
                frame_index.store(new_frame);
            }
        }
        play_btn->setChecked(false);
        play_btn->setIcon(QIcon(":/Icons/play.png"));
        set_status_bar("Pause");
    }
}

/**
 * @brief VideoWidget::update_tag
 * Update the Tag in the current label that corresponds to the current frame
 */
void VideoWidget::update_tag() {
    try {
        TagFrame* t_frame = m_tag->tag_map.at(playback_slider->value());
        VideoState state = m_vid_proj->get_video()->state;
        t_frame->set_center(state.center);
        t_frame->set_scale_rot(state.scale_factor, state.rotation, state.flip_h, state.flip_v);
        t_frame->update_color_correction(m_settings.brightness, m_settings.contrast,
                                         m_settings.gamma);
        emit set_status_bar("Frame number: " + QString::number(playback_slider->value()) + " updated");
    } catch (const std::out_of_range) {
        qWarning() << "Can't update. No tag found on current frame";
        return;
    }
}

void VideoWidget::update_tag_color(int b, double c, double g) {
    if (proj_tree_item == TAG_FRAME_ITEM) {
        m_tag->update_color_correction(playback_slider->value(), b, c, g);
        emit set_status_bar("Frame number: " + QString::number(playback_slider->value()) + " updated");
    } else if (proj_tree_item == TAG_ITEM || proj_tree_item == DRAWING_TAG_ITEM) {
        m_tag->update_color_whole_tag(b, c, g);
        emit set_status_bar("Whole tag '"+ m_tag->get_name() +"' updated");
    }
}

/**
 * @brief VideoWidget::tag_frame
 * Adds the current frame to a tag.
 */
void VideoWidget::tag_frame() {
    if (!tag_btn->isEnabled()) return;
    // If no tag is selected show the new tag dialog
    if (m_tag == nullptr || m_tag->is_drawing_tag()) {
        new_tag_clicked();
    }

    if (m_tag != nullptr && !m_tag->is_drawing_tag() && !frame_is_clean) {
        if (m_tag->find_frame(playback_slider->value())) {
            QMessageBox msg_box;
            msg_box.setText("Do you wanna overwrite the tag?");
            msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg_box.setDefaultButton(QMessageBox::No);
            int reply = msg_box.exec();
            if (reply == QMessageBox::No) return;
            update_tag();
        } else {
            // Add frame to tag
            VideoState state = m_vid_proj->get_video()->state;
            TagFrame* t_frame = new TagFrame(playback_slider->value(), state);
            m_tag->add_frame(playback_slider->value(), t_frame);
            emit tag_new_frame(playback_slider->value(), t_frame);
            emit set_status_bar("Frame number: " + QString::number(playback_slider->value()) + " tagged");
            playback_slider->update();
        }
    }
}

/**
 * @brief VideoWidget::remove_tag_frame
 * Un-tags the current frame
 */
void VideoWidget::remove_tag_frame() {
    if (m_tag != nullptr && !m_tag->is_drawing_tag()) {
        if (m_tag->find_frame(playback_slider->value())) {
            m_tag->remove_frame(playback_slider->value());
            emit tag_remove_frame(playback_slider->value());
            emit set_status_bar("Untagged frame number: " + QString::number(playback_slider->value()));
            playback_slider->update();
        } else {
            emit set_status_bar("Frame untagged");
        }
        return;
    }
    emit set_status_bar("Select a tag");
}

/**
 * @brief VideoWidget::new_tag_clicked
 * New-tag button clicked
 */
void VideoWidget::new_tag_clicked() {
    if (!tag_btn->isEnabled()) return;
    if (!m_vid_proj || frame_is_clean) return;
    TagDialog* tag_dialog = new TagDialog();
    tag_dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(tag_dialog, &TagDialog::tag_name, this, &VideoWidget::new_tag);
    tag_dialog->exec();
}

/**
 * @brief VideoWidget::new_tag
 * Creates a new tag
 * @param name
 */
void VideoWidget::new_tag(QString name) {
    Tag* tag = new Tag(name);
    emit add_tag(m_vid_proj, tag);
}

void VideoWidget::set_basic_analysis(BasicAnalysis *basic_analysis) {
    if (!basic_analysis) {
        m_current_interval = nullptr;
        m_tag = nullptr;
    } else if (basic_analysis->get_type() == INTERVAL) {
        m_current_interval = dynamic_cast<Interval*>(basic_analysis);
    } else {
        m_tag = dynamic_cast<Tag*>(basic_analysis);
    }
}

void VideoWidget::clear_tag() {
    m_tag = nullptr;
}

void VideoWidget::analysis_play_btn_toggled(bool value) {
    analysis_only = value;
    if (analysis_only) {
        int new_frame = playback_slider->get_closest_poi(frame_index.load());
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            frame_index.store(new_frame);
        }
        on_new_frame();
    }
}

/**
 * @brief VideoWidget::next_poi_btn_clicked
 * Jump to next detection aren on the slider
 */
void VideoWidget::next_poi_btn_clicked() {
    int current_frame = frame_index.load();
    int new_frame = playback_slider->get_next_poi_start(current_frame);
    if (new_frame != current_frame) {
        if (playback_slider->get_show_tags()) {
            VideoState state;
            state = playback_slider->m_tag->tag_map[new_frame]->get_state();
            load_marked_video_state(m_vid_proj, state);
        }
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            frame_index.store(new_frame);
        }
        on_new_frame();
        emit set_status_bar("Jumped to next POI");
    } else {
        emit set_status_bar("Already at last POI");
    }
}

/**
 * @brief VideoWidget::prev_poi_btn_clicked
 * * Jump to orevious detection aren on the slider
 */
void VideoWidget::prev_poi_btn_clicked() {
    int current_frame = frame_index.load();
    int new_frame = playback_slider->get_prev_poi_start(current_frame);
    if (new_frame != current_frame) {
        if (playback_slider->get_show_tags()) {
            VideoState state;
            state = playback_slider->m_tag->tag_map[new_frame]->get_state();
            load_marked_video_state(m_vid_proj, state);
        }
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            frame_index.store(new_frame);
        }
        on_new_frame();
        emit set_status_bar("Jumped to previous POI");
    } else {
        emit set_status_bar("Already at first POI");
    }
}

/**
 * @brief VideoWidget::display_index_slot
 * Slot function for on_new_frame that will jump to next poi
 * while analysis_only
 */
void VideoWidget::display_index_slot() {
    int frame_num = frame_index.load();
    auto interval = playback_slider->m_interval;

    int first;
    int second;
    if (loop && playback_slider->valid_interval(interval) && playback_slider->is_show_interval()) {
        first = interval.first;
        second = interval.second;
    } else {
        first = playback_slider->minimum();
        second = playback_slider->maximum();
    }

    frame_num = update_from_loop(frame_num);
    frame_num = update_from_ana_only(frame_num);

    if (frame_num > second || frame_num <first) {
        frame_num = update_from_loop(frame_num);
    }

    if (analysis_only) {
        if (loop && playback_slider->valid_interval(interval) && playback_slider->is_show_interval()) {
            if (frame_num > playback_slider->get_last_poi_end()) {
                frame_num = interval.first;
            }
        }

        int new_frame = update_from_ana_only(frame_num);
        if (new_frame > first && new_frame < second) {
            frame_num = new_frame;
        }
    }
    {
        std::lock_guard<std::mutex> p_lock(player_lock);
        frame_index.store(frame_num);
    }

    on_new_frame();
}

int VideoWidget::update_from_ana_only(int frame) {
    int new_frame = frame;
    if (analysis_only) {
        if (!playback_slider->is_in_POI(frame)) {
            new_frame = playback_slider->get_next_poi_start(frame);
        }
    }
    return new_frame;
}

int VideoWidget::update_from_loop(int frame) {
    int new_frame = frame;
    auto interval = playback_slider->m_interval;
    if (loop && playback_slider->valid_interval(interval) && playback_slider->is_show_interval()) {
        if (!playback_slider->is_in_interval(frame)) {
            new_frame = interval.first;
        }
    }
    return new_frame;
}

/**
 * @brief reacts to a new frame number. Updates the playback slider and current time label
 * @param frame_num
 */
void VideoWidget::on_new_frame() {
    int frame_num = frame_index.load();
    if (frame_num == m_frame_length - 1) play_btn->setChecked(false);
    if (!playback_slider->is_blocked()) {
        // Block signals to prevent value_changed signal to trigger
        playback_slider->blockSignals(true);
        playback_slider->setValue(frame_num);
        playback_slider->blockSignals(false);
    }
    if (analysis_only && !(loop && playback_slider->is_show_interval())) {
        if (!playback_slider->is_in_POI(frame_num)) {
            if (frame_num >= playback_slider->get_last_poi_end()) {
                play_btn->setChecked(false);
            }
        }
    }

    if (m_frame_rate) set_current_time(frame_num / m_frame_rate);
    frame_line_edit->setText(QString::number(frame_num));

    if (!m_floating) {
        if (proj_tree_item == VIDEO_ITEM || proj_tree_item == ANALYSIS_ITEM) {
            m_vid_proj->state.frame = frame_num;
        }
        m_vid_proj->get_video()->state.frame = frame_num;
    }

    playback_slider->update();
    player_con.notify_one();
}

/**
 * @brief VideoWidget::on_playback_slider_pressed
 */
void VideoWidget::on_playback_slider_pressed() {
    timer.start();
    playback_slider->set_blocked(true);
}

/**
 * @brief VideoWidget::on_playback_slider_released
 */
void VideoWidget::on_playback_slider_released() {
    playback_slider->set_blocked(false);
    int new_frame = playback_slider->value();
    if (analysis_only) {
        new_frame = playback_slider->get_closest_poi(new_frame);
    }
    {
        std::lock_guard<std::mutex> p_lock(player_lock);
        frame_index.store(new_frame);
    }
    on_new_frame();
}

/**
 * @brief VideoWidget::on_playback_slider_value_changed
 */
void VideoWidget::on_playback_slider_value_changed() {
    int new_frame = playback_slider->value();
    if (analysis_only && !playback_slider->is_blocked()) {
        new_frame = playback_slider->get_closest_poi(new_frame);
    }
    {
        std::lock_guard<std::mutex> p_lock(player_lock);
        frame_index.store(new_frame);
    }
    on_new_frame();
}

/**
 * @brief VideoWidget::on_playback_slider_moved
 */
void VideoWidget::on_playback_slider_moved() {
    if (std::abs(playback_slider->value() - prev_frame_idx) % 5 == 0) {
        frame_index.store(playback_slider->value());
    }

    if (timer.elapsed() > 200) {
        frame_index.store(playback_slider->value());
        timer.restart();
    }
    on_new_frame();
}

/**
 * @brief VideoWidget::load_marked_video_state
 * Slot function for loading a new video
 * @param vid_proj
 */
void VideoWidget::load_marked_video_state(VideoProject* vid_proj, VideoState state) {
    set_seq_tag_btns(false);
    if (!video_btns_enabled) set_video_btns(true);
    if (!vid_proj->is_current() || m_vid_proj == nullptr) {
        if (m_vid_proj) m_vid_proj->set_current(false);
        vid_proj->set_current(true);
        m_vid_proj = vid_proj;
        set_interval(vid_proj->m_interval);
        frame_is_clean = false;
        frame_wgt->clear_frame(false);

        // Set state variables but don't update the processor
        {
            std::lock_guard<std::mutex> v_lock(v_sync.lock);
            z_settings.set_state = true;
            z_settings.anchor = state.anchor;
            z_settings.center = state.center;
            z_settings.zoom_factor = state.scale_factor;
            z_settings.rotation = state.rotation;
            z_settings.flip_state_h = state.flip_h;
            z_settings.flip_state_v = state.flip_v;
            m_settings.brightness = state.brightness;
            m_settings.contrast = state.contrast;
            m_settings.gamma = state.gamma;
            o_settings.overlay = m_vid_proj->get_overlay();
        }

        // Set new video information and notify player
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            v_sync.frame_index_on_load = state.frame;
            m_video_path = vid_proj->get_video()->file_path;
            new_video.store(true);
        }
        player_con.notify_all();

    } else {
        if (frame_index.load() != state.frame) {
            std::lock_guard<std::mutex> v_lock(v_sync.lock);
            m_frame_changing.store(true);
        }
        set_state(state);
        if (state.frame > -1) {
            on_new_frame();
        }
        set_video_btns(!frame_is_clean);
        if (vid_proj->get_video()->get_sequence_type() == TAG_SEQUENCE) set_seq_tag_btns(true);
    }
    set_status_bar("Video loaded");
    play_btn->setChecked(false);
    emit update_manipulator_wgt(state.brightness, state.contrast, state.gamma);
}

/**
 * @brief VideoWidget::set_item_type
 * Save the item type of the clicked item in the project tree
 * @param type
 */
void VideoWidget::set_item_type(int type) {
    proj_tree_item = type;
}

void VideoWidget::remove_item(VideoProject* vid_proj) {
    if (get_current_video_project() == vid_proj) clear_current_video();
}

/**
 * @brief VideoWidget::clear_current_video
 * Removes the video from the videoplayer
 */
void VideoWidget::clear_current_video() {
    int frame = -1;
    if (video_btns_enabled) set_video_btns(false);

    player_lock.lock();
    video_loaded.store(false);
    player_lock.unlock();
    player_con.notify_all();

    playback_slider->blockSignals(true);
    playback_slider->setValue(frame);
    playback_slider->blockSignals(false);
    play_btn->setChecked(false);
    delete_interval();
    set_total_time(0);
    video_label->setText("");
    fps_label->setText("Fps: -");
    max_frames->setText("/ -");
    zoom_label->setText("100%");

    frame_wgt->close();
    m_vid_proj = nullptr;
    m_tag = nullptr;
    m_current_interval = nullptr;
    emit clean_zoom_preview();
}

void VideoWidget::set_video_btns(bool b) {
    for (QPushButton* btn : btns) {
        btn->setEnabled(b);
    }
    interpolate_check->setEnabled(b);
    playback_slider->setEnabled(b);
    frame_line_edit->setEnabled(b);
    zoom_label->setEnabled(b);
    speed_slider->setEnabled(b);
    tag_btn->setEnabled(b);
    video_btns_enabled = b;
    if (m_floating) {
        bookmark_btn->setDisabled(true);
        new_label_btn->setDisabled(true);
        tag_btn->setDisabled(true);
        export_frame_btn->setDisabled(true);
        set_start_interval_btn->setDisabled(true);
        set_end_interval_btn->setDisabled(true);
    }
}

void VideoWidget::enable_poi_btns(bool b, bool ana_play_btn) {
    next_poi_btn->setEnabled(b);
    prev_poi_btn->setEnabled(b);

    analysis_play_btn->setEnabled(ana_play_btn);
    if (!ana_play_btn) {
        analysis_play_btn->setChecked(ana_play_btn);
        analysis_only = ana_play_btn;
    }
}

void VideoWidget::set_seq_tag_btns(bool value) {
    play_btn->setDisabled(value);
    stop_btn->setDisabled(value);
    next_frame_btn->setDisabled(value);
    prev_frame_btn->setDisabled(value);
    new_label_btn->setDisabled(value);
    set_start_interval_btn->setDisabled(value);
    set_end_interval_btn->setDisabled(value);
    create_interval_btn->setDisabled(value);
    tag_btn->setDisabled(value);
    speed_slider->setDisabled(value);
}

void VideoWidget::update_video_label(QString name) {
    video_label->setText(name);
}

/**
 * @brief VideoWidget::capture_failed
 * Open a dialog where the user can enter a new path to the video.
 * Will then try to open the video again.
 */
void VideoWidget::capture_failed() {
    PathDialog* dialog = new PathDialog(&m_video_path);
    connect(dialog, &PathDialog::open_view_path_dialog, this, &VideoWidget::open_view_path_dialog);

    int status = dialog->exec();

    if (status == dialog->Accepted) {
        {
            std::lock_guard<std::mutex> p_lock(player_lock);
            m_vid_proj->get_video()->file_path = m_video_path;
            new_video.store(true);
        }
        player_con.notify_all();
    } else {
        frame_wgt->clear_frame(true);
        frame_is_clean = true;
        set_video_btns(false);

        // Set variable and send to processor
        set_no_video();
    }
    emit update_videoitem(m_video_path);
}

/**
 * @brief VideoWidget::on_video_info
 * @param video_width
 * @param video_height
 * @param frame_rate
 * @param last_frame
 * notified of new settings when videoplayer has loaded a new video
 */
void VideoWidget::on_video_info(int video_width, int video_height, int frame_rate, int last_frame){
    int current_frame_index = frame_index.load();

    m_vid_proj->get_video()->set_size(video_width, video_height);
    m_vid_proj->get_video()->set_frame_rate(frame_rate);
    m_vid_proj->get_video()->set_last_frame(last_frame);
    m_frame_rate = frame_rate;
    m_frame_length = last_frame + 1;
    current_frame_size = QSize(video_width, video_height);
    set_frame_size(video_width, video_height);

    // Solves a bug where the setMaximum will set the frame index to max in some cases
    set_slider_max(last_frame);
    playback_slider->blockSignals(true);
    playback_slider->setValue(current_frame_index);
    playback_slider->blockSignals(false);

    if (frame_rate != 0) {
        set_total_time((last_frame + 1) / frame_rate);
        set_current_time(current_frame_index / frame_rate);
    } else {
        set_total_time(0);
        set_current_time(0);
    }
    fps_label->setText(QString::number(frame_rate) + "fps");

    on_new_frame();
}

void VideoWidget::set_slider_max(int max) {
    playback_slider->blockSignals(true);
    if (max == -1) {
        playback_slider->setMaximum(playback_slider->maximum()-1);
    } else {
        playback_slider->setMaximum(max);
    }
    playback_slider->blockSignals(false);
    max_frames->setText("/ " + QString::number(playback_slider->maximum()));
}

void VideoWidget::set_frame_size(int width, int height) {
    m_video_width = width;
    m_video_height = height;
    size_label->setText("(" + QString::number(width) + "x" + QString::number(height) + ")");
}

void VideoWidget::on_playback_stopped(){
    play_btn->setChecked(false);
}

/**
 *  Functions changing synchronized settings
 */

void VideoWidget::set_overlay_removed() {
    update_overlay_settings([&](){
        o_settings.overlay_removed = true;
    });
}

void VideoWidget::set_update_text(QString text, Shapes* shape) {
    update_overlay_settings([&](){
        o_settings.update_text = true;
        o_settings.shape = shape;
        o_settings.text = text;
    });
}

void VideoWidget::set_clear_drawings(int frame) {
    update_overlay_settings([&](){
        o_settings.clear_drawings = true;
        o_settings.frame = frame;
    });
}

void VideoWidget::set_delete_drawing(Shapes *shape) {
    update_overlay_settings([&](){
        o_settings.delete_drawing = true;
        o_settings.shape = shape;
    });
}

void VideoWidget::set_show_overlay(bool show) {
    update_overlay_settings([&](){
        o_settings.show_overlay = show;
    });
}

void VideoWidget::set_tool(SHAPES tool) {
    update_overlay_settings([&](){
        o_settings.tool = tool;
    });
}

void VideoWidget::set_tool_text(QString text, float font_scale) {
    update_overlay_settings([&](){
        o_settings.create_text = true;
        o_settings.current_string = text;
        o_settings.current_font_scale = font_scale;
    });
}

void VideoWidget::set_color(QColor color) {
    update_overlay_settings([&](){
        o_settings.color = color;
    });
}

void VideoWidget::mouse_double_clicked(QPoint pos) {
    update_overlay_settings([&](){
        o_settings.mouse_double_clicked = true;
        o_settings.pos = pos;
    });
}

void VideoWidget::mouse_pressed(QPoint pos, bool right_click) {
    if (is_playing.load()) {
        play_btn_toggled(false);
    }
    update_overlay_settings([&](){
        o_settings.mouse_clicked = true;
        o_settings.pos = pos;
        o_settings.right_click = right_click;
    });
}

void VideoWidget::mouse_released(QPoint pos, bool right_click) {
    update_overlay_settings([&](){
        o_settings.mouse_released = true;
        o_settings.pos = pos;
        o_settings.right_click = right_click;
    });
}

void VideoWidget::mouse_moved(QPoint pos, bool shift, bool ctrl) {
    update_overlay_settings([&](){
        o_settings.mouse_moved = true;
        o_settings.shift_modifier = shift;
        o_settings.ctrl_modifier = ctrl;
        o_settings.pos = pos;
    });
}

void VideoWidget::mouse_scroll(QPoint pos) {
    update_overlay_settings([&](){
        o_settings.mouse_scroll = true;
        o_settings.pos = pos;
    });
}

void VideoWidget::set_current_drawing(Shapes* shape) {
    update_overlay_settings([&](){
        o_settings.set_current_drawing = true;
        o_settings.shape = shape;
    });
}

void VideoWidget::set_no_video() {
    update_overlay_settings([&](){
        o_settings.no_video = true;
    });
}

void VideoWidget::process_frame() {
    update_overlay_settings([&](){
        o_settings.copy_paste = true;
    });
}

/**
 * @brief VideoWidget::update_overlay_settings
 * This functions intended use is to update variables shared with the frame processor thread.
 * After the change is made it will notify the frame processor.
 * @param lambda function where the variable is changed
 */
void VideoWidget::update_overlay_settings(std::function<void ()> lambda) {
    v_sync.lock.lock();
    lambda();
    overlay_changed.store(true);
    v_sync.lock.unlock();
    v_sync.con_var.notify_all();
}

/**
 * @brief VideoWidget::pan
 * Notifies the frame processor when the zoom rectangle should be moved
 * @param x movement
 * @param y movement
 */
void VideoWidget::pan(int x, int y) {
    update_processing_settings([&](){
        z_settings.x_movement = x;
        z_settings.y_movement = y;
    });
}

/**
 * @brief VideoWidget::center
 * Notifies the frame processor when the zoom rectangle should be centered
 * @param center
 * @param
 */
void VideoWidget::center(QPoint pos, double zoom_step) {
    update_processing_settings([&](){
        z_settings.center = pos;
        z_settings.do_point_zoom = true;
        z_settings.zoom_step = zoom_step;
    });
}

/**
 * @brief VideoWidget::set_zoom_area
 * Notifies the frame processor that a new zoom area has been set
 * @param p1 tl point of the zoom area
 * @param p2 br point of the zoom area
 */
void VideoWidget::set_zoom_area(QPoint p1, QPoint p2) {
    update_processing_settings([&](){
        z_settings.zoom_area_tl = p1;
        z_settings.zoom_area_br = p2;
        z_settings.has_new_zoom_area = true;
    });
}

/**
 * @brief VideoWidget::set_draw_area_size
 * Notifies the frame processor that the draw area has changed size
 * @param s Size of the area where the frame will be displayed
 */
void VideoWidget::set_draw_area_size(QSize s) {
    update_processing_settings([&](){z_settings.draw_area_size = s;});
}

/**
 * @brief VideoWidget::set_interpolation_method
 * Notifies the frame processor that the interpolation method has been changed
 * @param method - valid opencv interpolation constant
 */
void VideoWidget::set_interpolation_method(int method) {
    update_processing_settings([&](){
        z_settings.interpolation = method;
    });
}

/**
 * @brief VideoWidget::on_zoom_out
 * Tells the frame processor to change the zoom with a constant
 */
void VideoWidget::on_step_zoom(double step){
    update_processing_settings([&](){z_settings.zoom_factor *= step;});
}

void VideoWidget::set_zoom_factor(double scale_factor) {
    update_processing_settings([&](){z_settings.zoom_factor = scale_factor;});
}

/**
 * @brief VideoWidget::set_state
 * Tells the frame processor to set the state of the video.
 * Anchor, scale factor and frame number
 * @param state
 */
void VideoWidget::set_state(VideoState state) {
    update_processing_settings([&](){
        z_settings.set_state = true;
        z_settings.anchor = state.anchor;
        z_settings.center = state.center;
        z_settings.zoom_factor = state.scale_factor;
        z_settings.rotation = state.rotation;
        z_settings.flip_state_h = state.flip_h;
        z_settings.flip_state_v = state.flip_v;
        // TODO Maybe test some more to see if this really isn't needed anymore
        // Now with the new flag variable in the frameprocessor it shouldn't be needed
        // It it's removed, remove the check from the processor as well
        //z_settings.skip_frame_refresh = frame_index.load() != state.frame;
        m_settings.brightness = state.brightness;
        m_settings.contrast = state.contrast;
        m_settings.gamma = state.gamma;
        frame_index.store(state.frame);
    });
}

/**
 * @brief VideoWidget::on_fit_screen
 * Tells the frame processor to fit the video to the current draw area
 */
void VideoWidget::on_fit_screen() {
    update_processing_settings([&](){z_settings.fit = true;});
}

/**
 * @brief VideoWidget::on_original_size
 * Tells the frame processor to display the video in its original size
 */
void VideoWidget::on_original_size(){
    update_processing_settings([&](){z_settings.original = true;});
}

/**
 * @brief VideoWidget::update_brightness_contrast
 * Notifies the frame processor of changes to the brightness and contrast values
 * @param b_val brightness value
 * @param c_val contrast value
 */
void VideoWidget::update_brightness_contrast(int b_val, double c_val, double g_val, bool update) {
    if (proj_tree_item == VIDEO_ITEM) update = true;
    update_processing_settings([&](){
        m_settings.brightness = b_val;
        m_settings.contrast = c_val;
        m_settings.gamma = g_val;
        m_settings.update_state = update;
    });
}

/**
 * @brief VideoWidget::rotate_cw
 * Tells the frame processor to rotate the frame cw
 */
void VideoWidget::rotate_cw() {
    update_processing_settings([&](){m_settings.rotate = 1;});
}

/**
 * @brief VideoWidget::rotate_ccw
 * Tells the frame processor to rotate the frame ccw
 */
void VideoWidget::rotate_ccw() {
    update_processing_settings([&](){m_settings.rotate = -1;});
}

void VideoWidget::flip_horizontal() {
    update_processing_settings([&](){
        m_settings.do_flip_h = true;
    });
}

void VideoWidget::flip_vertical() {
    update_processing_settings([&](){
        m_settings.do_flip_v = true;
    });
}

/**
 * @brief VideoWidget::update_processing_settings
 * This functions intended use is to update variables shared with the frame processor thread.
 * After the change is made it will notify the frame processor.
 * @param lambda function where the variable is changed
 */
void VideoWidget::update_processing_settings(std::function<void ()> lambda) {
    if (!frame_wgt->isHidden()) {
        v_sync.lock.lock();
        lambda();
        settings_changed.store(true);
        v_sync.lock.unlock();
        v_sync.con_var.notify_all();
    } else {
        lambda();
    }
}

void VideoWidget::update_playback_speed(int speed) {
    {
        std::lock_guard<std::mutex> lk(player_lock);
        m_speed_step.store(speed);
        player_con.notify_one();
    }
    fps_label->setText(QString::number(m_frame_rate) + "fps");
}

void VideoWidget::on_export_frame() {
    if (frame_is_clean) return;
    int frame = frame_index.load();
    // Get sequence item and send that one to get the hash
    emit export_original_frame(m_vid_proj, frame, frame_wgt->get_org_frame());
    emit set_status_bar(QString("Frame %1 exported").arg(frame));
}

/**
 * @brief VideoWidget::frame_line_edit_finished
 * Slot function for when the frame_line_edit is finished edited and enter is pressed
 * Checks if input is a legal frame and then set that as current frame.
 */
void VideoWidget::frame_line_edit_finished() {
    std::string text = frame_line_edit->text().toStdString();
    char* p;
    long converted = strtol(text.c_str(), &p, 10);
    if (*p != 0){
        emit set_status_bar("Error! Input is not a number!");
    } else if (converted > playback_slider->maximum()) {
        QString num_frames = QString::number(playback_slider->maximum());
        emit set_status_bar("Error! Input is too large. " + num_frames + " is max frame number.");
    } else if (converted < 0) {
        emit set_status_bar("Error! Input is negative!");
    } else {
        frame_index.store(converted);
        on_new_frame();
    }
    frame_line_edit->clearFocus();
}

/**
 * @brief VideoWidget::zoom_label_finished
 * Slot function for when the zoom_label is finished edited and enter is pressed
 * Checks if input is a legal scale factor and the sends it to the processor.
 */
void VideoWidget::zoom_label_finished() {
    std::string text = zoom_label->text().toStdString();
    text.erase(std::remove(text.begin(), text.end(), '%'), text.end());
    char* p;
    double converted = strtod(text.c_str(), &p)/Constants::DOUBLE_TO_INT;
    if (*p != 0) {
        emit set_status_bar("Error! Input is not a number!");
    } else if (converted < 0) {
        emit set_status_bar("Error! Input is negative!");
    } else if (converted < static_cast<double>(Constants::ZOOM_LABEL_MIN) / Constants::DOUBLE_TO_INT) {
        emit set_status_bar("Error! Input is too small");
    } else if (converted > static_cast<double>(Constants::ZOOM_LABEL_MAX) / Constants::DOUBLE_TO_INT) {
        emit set_status_bar("Error! Input is too large");
    } else {
        set_zoom_factor(converted);
        zoom_label->setText(QString::number(converted*Constants::DOUBLE_TO_INT) + "%");
        zoom_label->clearFocus();
        return;
    }
    zoom_label->setText(QString::number(m_scale_factor*Constants::DOUBLE_TO_INT) + "%");
    zoom_label->clearFocus();
}

void VideoWidget::set_brightness_contrast(int bri, double cont, double gamma) {
    if (!m_vid_proj) return;
    if (proj_tree_item == VIDEO_ITEM) {
        m_vid_proj->state.brightness = bri;
        m_vid_proj->state.contrast = cont;
        m_vid_proj->state.gamma = gamma;
    }
    m_vid_proj->get_video()->state.brightness = bri;
    m_vid_proj->get_video()->state.contrast = cont;
    m_vid_proj->get_video()->state.gamma = gamma;
}

void VideoWidget::update_zoom_preview_size(QSize s) {
    update_processing_settings([&](){
        z_settings.preview_window_size = s;
    });
}

/**
 * @brief VideoWidget::translate_zoom_from_preview_click
 * Scales the click coordinates (from zoom preview) to new coordinates in the original frame
 * and updates the frame processor so that the viewport will center around that position.
 * @param click_pos:    The coordinate where the click occured (zoom preview coordinates)
 */
void VideoWidget::translate_zoom_from_preview_click(QPoint click_pos) {
    update_processing_settings([&](){
        double scale_factor =  z_settings.frame_size.width() / static_cast<double>(z_settings.preview_window_size.width());
        QPoint scaled = click_pos * scale_factor;
        z_settings.x_movement = scaled.x();
        z_settings.y_movement = scaled.y();
    });
}

void VideoWidget::speed_up_activate() {
    if (speed_slider->value() == speed_slider->maximum()) return;
    speed_slider->setValue(speed_slider->value() +1);
}

void VideoWidget::speed_down_activate() {
    if (speed_slider->value() == speed_slider->minimum()) return;
    speed_slider->setValue(speed_slider->value() -1);
}

void VideoWidget::set_video_start() {
    if (!video_btns_enabled) return;
    frame_index.store(playback_slider->minimum());
    on_new_frame();
}

void VideoWidget::set_video_end() {
    if (!video_btns_enabled) return;
    frame_index.store(playback_slider->maximum());
    on_new_frame();
}

void VideoWidget::page_step_front() {
    if (!video_btns_enabled) return;
    int value = playback_slider->value();
    Singleton* s = Singleton::get_instance();
    if (value + s->PAGE_STEP > playback_slider->maximum()) {
        frame_index.store(playback_slider->maximum());
    } else {
        frame_index.store(value + s->PAGE_STEP);
    }
    on_new_frame();
}

void VideoWidget::page_step_back() {
    if (!video_btns_enabled) return;
    int value = playback_slider->value();
    Singleton* s = Singleton::get_instance();
    if (value - s->PAGE_STEP < playback_slider->minimum()) {
        frame_index.store(playback_slider->minimum());
    } else {
        frame_index.store(value - s->PAGE_STEP);
    }
    on_new_frame();
}

void VideoWidget::frame_label_focus() {
    frame_line_edit->setFocus();
    frame_line_edit->selectAll();
}

void VideoWidget::zoom_label_focus() {
    zoom_label->setFocus();
    zoom_label->selectAll();
}
