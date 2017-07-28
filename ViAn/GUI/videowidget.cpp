#include "videowidget.h"
#include "utility.h"
#include "drawscrollarea.h"
#include "tagdialog.h"

#include <QTime>
#include <QDebug>
#include <QShortcut>
#include <QScrollBar>
#include <QProgressDialog>

#include "GUI/frameexporterdialog.h"
#include "Video/video_player.h"
#include "Analysis/AnalysisController.h"
#include "imageexporter.h"

#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>



VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent), scroll_area(new DrawScrollArea) {
    // Init video contoller
    v_controller = new VideoController(&frame_index, &is_playing);

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

    connect(scroll_area, SIGNAL(new_size(QSize)), frame_wgt, SLOT(set_scroll_area_size(QSize)));
    connect(this, SIGNAL(set_detections_on_frame(int)), frame_wgt, SLOT(set_detections_on_frame(int)));
    init_video_controller();
    v_controller->start();
}

VideoProject *VideoWidget::get_current_video_project(){
    return m_vid_proj;
}

std::pair<int, int> VideoWidget::get_frame_interval(){
    return m_interval;
}

/**
 * @brief VideoWidget::get_current_video_length
 * @return The length of the current video in frames
 */
int VideoWidget::get_current_video_length(){
    return m_frame_length;
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
    // Playback control
    connect(speed_slider, SIGNAL(valueChanged(int)), v_controller, SIGNAL(update_speed(int)));
    connect(this, &VideoWidget::load_video, v_controller, &VideoController::load_video);

    // Zoom control
    connect(frame_wgt, SIGNAL(zoom_points(QPoint, QPoint)), v_controller, SIGNAL(set_zoom_rect(QPoint, QPoint)));
    connect(scroll_area, SIGNAL(new_size(QSize)), v_controller, SIGNAL(set_draw_area_size(QSize)));
    connect(zoom_out_btn, &QPushButton::clicked, v_controller, &VideoController::zoom_out);
    connect(fit_btn, &QPushButton::clicked, v_controller, &VideoController::fit_screen);
    connect(original_size_btn, &QPushButton::clicked, v_controller, &VideoController::original_size);
    connect(frame_wgt, SIGNAL(moved_xy(int,int)), v_controller, SIGNAL(move_zoom_rect(int,int)));

    // Video data
    connect(v_controller, &VideoController::display_image, frame_wgt, &FrameWidget::on_new_image);
    connect(v_controller, &VideoController::video_info, this, &VideoWidget::on_video_info);
    connect(v_controller, SIGNAL(display_index()), this, SLOT(on_new_frame()));
    connect(v_controller, &VideoController::playback_stopped, this, &VideoWidget::on_playback_stopped);

    connect(v_controller, SIGNAL(scale_factor(double)), frame_wgt, SLOT(set_scale_factor(double)));
    connect(v_controller, SIGNAL(scale_factor(double)), this, SLOT(set_scale_factor(double)));
    connect(v_controller, SIGNAL(anchor(QPoint)), frame_wgt, SLOT(set_anchor(QPoint)));
}

/**
 * @brief VideoWidget::set_icons
 * Set icons on all buttons
 */
void VideoWidget::set_btn_icons() {
    play_btn = new QPushButton(QIcon("../ViAn/Icons/play.png"), "", this);
    stop_btn = new QPushButton(QIcon("../ViAn/Icons/stop.png"), "", this);
    next_frame_btn = new QPushButton(QIcon("../ViAn/Icons/next_frame.png"), "", this);
    prev_frame_btn = new QPushButton(QIcon("../ViAn/Icons/prev_frame.png"), "", this);
    next_poi_btn = new QPushButton(QIcon("../ViAn/Icons/next_poi.png"), "", this);
    prev_poi_btn = new QPushButton(QIcon("../ViAn/Icons/prev_poi.png"), "", this);
    bookmark_btn = new QPushButton(QIcon("../ViAn/Icons/bookmark.png"), "", this);
    analysis_btn = new QPushButton(QIcon("../ViAn/Icons/analysis.png"), "", this);
    analysis_play_btn = new QPushButton(QIcon("../ViAn/Icons/play.png"), "", this);
    tag_btn = new QPushButton(QIcon("../ViAn/Icons/tag.png"), "", this);
    new_tag_btn = new QPushButton(QIcon("../ViAn/Icons/marker.png"), "", this);

    zoom_in_btn = new QPushButton(QIcon("../ViAn/Icons/zoom_in.png"), "", this);
    zoom_out_btn = new QPushButton(QIcon("../ViAn/Icons/zoom_out.png"), "", this);
    fit_btn = new QPushButton(QIcon("../ViAn/Icons/fit_screen.png"), "", this);
    original_size_btn = new QPushButton(QIcon("../ViAn/Icons/move.png"), "", this);
    zoom_label = new QLabel;
    zoom_label->setText("100%");
    zoom_label->setMinimumWidth(40);
    set_start_interval_btn = new QPushButton(QIcon("../ViAn/Icons/start_interval.png"), "", this);
    set_end_interval_btn = new QPushButton(QIcon("../ViAn/Icons/end_interval.png"), "", this);
    play_btn->setCheckable(true);
    zoom_in_btn->setCheckable(true);
    analysis_play_btn->setCheckable(true);
}

/**
 * @brief VideoWidget::set_tool_tip
 * Set tooltip on all buttons
 */
void VideoWidget::set_btn_tool_tip() {
    play_btn->setToolTip(tr("Play video"));
    stop_btn->setToolTip(tr("Stop video"));
    next_frame_btn->setToolTip(tr("Next frame"));
    prev_frame_btn->setToolTip(tr("Previous frame"));
    next_poi_btn->setToolTip(tr("Next POI"));
    prev_poi_btn->setToolTip(tr("Previous POI"));
    analysis_btn->setToolTip(tr("Analysis"));
    analysis_play_btn->setToolTip(tr("Play only the POIs"));
    bookmark_btn->setToolTip(tr("Bookmark the current frame"));
    tag_btn->setToolTip(tr("Tag the current frame"));
    new_tag_btn->setToolTip(tr("Create a new tag"));
    zoom_in_btn->setToolTip(tr("Zoom in"));
    zoom_out_btn->setToolTip(tr("Zoom out"));
    fit_btn->setToolTip(tr("Scale the video to screen"));
    original_size_btn->setToolTip(tr("Scale video to original size"));
    set_start_interval_btn->setToolTip("Set left interval point");
    set_end_interval_btn->setToolTip("Set right interval point");
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
    btns.push_back(analysis_btn);
    btns.push_back(new_tag_btn);
    btns.push_back(zoom_in_btn);
    btns.push_back(zoom_out_btn);
    btns.push_back(fit_btn);
    btns.push_back(original_size_btn);
    btns.push_back(set_start_interval_btn);
    btns.push_back(set_end_interval_btn);

    for (QPushButton* btn : btns) {
        btn->setFixedSize(BTN_SIZE);
        btn->setEnabled(false);
    }
    next_poi_btn->setFixedSize(BTN_SIZE);
    prev_poi_btn->setFixedSize(BTN_SIZE);
    tag_btn->setFixedSize(BTN_SIZE);
    analysis_play_btn->setFixedSize(BTN_SIZE);
    enable_poi_btns(false,false);
    enable_tag_btn(false);
}

/**
 * @brief VideoWidget::set_tab_order
 * Set the tab order for the buttons
 */
void VideoWidget::set_btn_tab_order() {
    // TODO update

    setTabOrder(prev_frame_btn, play_btn);
    setTabOrder(play_btn, next_frame_btn);
    setTabOrder(next_frame_btn, stop_btn);
    setTabOrder(stop_btn, prev_poi_btn);
    setTabOrder(prev_poi_btn, analysis_btn);
    setTabOrder(analysis_btn, next_poi_btn);
    setTabOrder(next_poi_btn, bookmark_btn);
    setTabOrder(bookmark_btn, tag_btn);
    setTabOrder(tag_btn, zoom_in_btn);
    setTabOrder(zoom_in_btn, zoom_out_btn);
    setTabOrder(zoom_out_btn, fit_btn);
    setTabOrder(fit_btn, original_size_btn);
}

/**
 * @brief VideoWidget::set_btn_shortcuts
 * Set shortcuts to the buttons
 */
void VideoWidget::set_btn_shortcuts() {
    play_btn->setShortcut(Qt::Key_Space);
    stop_btn->setShortcut(Qt::Key_X);
    next_frame_btn->setShortcut(Qt::Key_Right);
    prev_frame_btn->setShortcut(Qt::Key_Left);
    next_poi_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    prev_poi_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    analysis_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    bookmark_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    tag_btn->setShortcut(Qt::Key_T);
    remove_frame_act = new QShortcut(Qt::Key_R, this);
    new_tag_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    zoom_in_btn->setShortcut(Qt::Key_Z);
    fit_btn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    set_start_interval_btn->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Left));
    set_end_interval_btn->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Right));
}

/**
 * @brief VideoWidget::init_speed_slider
 * Create and add speed adjustment slider
 */
void VideoWidget::init_speed_slider() {
    speed_slider = new QSlider(Qt::Horizontal);
    speed_slider->setRange(-4,4);
    speed_slider->setMaximumWidth(120);
    speed_slider->setPageStep(1);
    speed_slider->setTickPosition(QSlider::TicksBelow);
    speed_slider->setEnabled(false);
    speed_slider->setToolTip(tr("Adjust playback speed"));
    QLabel *label1 = new QLabel("1/16x", this);
    QLabel *label2 = new QLabel("1x", this);
    QLabel *label3 = new QLabel("16x", this);
    QFont f("Helvetica", 6, QFont::Normal);
    label1->setFont(f);
    label2->setFont(f);
    label3->setFont(f);
    //laout->addWidget(widget, int row, int column, int rowSpan, int columnSpan)
    speed_slider_layout->addWidget(speed_slider, 0, 0, 1, 5);
    speed_slider_layout->addWidget(label1, 1, 0, 1, 1);
    speed_slider_layout->addWidget(label2, 1, 2, 1, 1);
    speed_slider_layout->addWidget(label3, 1, 4, 1, 1);
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
    analysis_btns->addWidget(analysis_btn);
    analysis_btns->addWidget(next_poi_btn);
    analysis_btns->addWidget(analysis_play_btn);

    control_row->addLayout(analysis_btns);

    other_btns->addWidget(bookmark_btn);
    other_btns->addWidget(tag_btn);
    other_btns->addWidget(new_tag_btn);

    control_row->addLayout(other_btns);

    zoom_btns->addWidget(zoom_in_btn);
    zoom_btns->addWidget(zoom_out_btn);
    zoom_btns->addWidget(fit_btn);
    zoom_btns->addWidget(original_size_btn);
    zoom_btns->addWidget(zoom_label);

    control_row->addLayout(zoom_btns);

    interval_btns->addWidget(set_start_interval_btn);
    interval_btns->addWidget(set_end_interval_btn);

    control_row->addLayout(interval_btns);

    vertical_layout->addLayout(control_row);
}

/**
 * @brief VideoWidget::connect_btns
 * Connect all control buttons with signals and slots
 */
void VideoWidget::connect_btns() {
    connect(play_btn, &QPushButton::toggled, this, &VideoWidget::play_btn_toggled);
    connect(stop_btn, &QPushButton::clicked, this, &VideoWidget::stop_btn_clicked);
    connect(next_frame_btn, &QPushButton::clicked, this, &VideoWidget::next_frame_clicked);
    connect(prev_frame_btn, &QPushButton::clicked, this, &VideoWidget::prev_frame_clicked);

    connect(analysis_btn, &QPushButton::clicked, this, &VideoWidget::analysis_btn_clicked);
    connect(analysis_play_btn, &QPushButton::toggled, this, &VideoWidget::analysis_play_btn_toggled);

    connect(next_poi_btn, &QPushButton::clicked, this, &VideoWidget::next_poi_btn_clicked);
    connect(prev_poi_btn, &QPushButton::clicked, this, &VideoWidget::prev_poi_btn_clicked);

    connect(zoom_in_btn, &QPushButton::toggled, frame_wgt, &FrameWidget::toggle_zoom);

    connect(bookmark_btn, &QPushButton::clicked, this, &VideoWidget::on_bookmark_clicked);
    connect(tag_btn, &QPushButton::clicked, this, &VideoWidget::tag_frame);
    connect(remove_frame_act, &QShortcut::activated, this, &VideoWidget::remove_tag_frame);
    connect(new_tag_btn, &QPushButton::clicked, this, &VideoWidget::new_tag_clicked);

    connect(frame_wgt, &FrameWidget::trigger_zoom_out, zoom_out_btn, &QPushButton::click);

    connect(set_start_interval_btn, &QPushButton::clicked, this, &VideoWidget::set_interval_start_clicked);
    connect(set_end_interval_btn, &QPushButton::clicked, this, &VideoWidget::set_interval_end_clicked);
}

/**
 * @brief Adds the video slider and playback timestamps
 */
void VideoWidget::init_playback_slider() {
    QHBoxLayout* progress_area = new QHBoxLayout();
    current_time = new QLabel("--:--");
    total_time = new QLabel("--:--");
    frame_line_edit = new QLineEdit("0");

    frame_line_edit->setFixedWidth(50);

    playback_slider = new AnalysisSlider(Qt::Horizontal);

    frame_line_edit->setEnabled(false);
    playback_slider->setEnabled(false);
    progress_area->addWidget(current_time);
    progress_area->addWidget(playback_slider);
    progress_area->addWidget(total_time);
    progress_area->addWidget(frame_line_edit);
    vertical_layout->addLayout(progress_area);

    connect(playback_slider, &QSlider::sliderPressed, this, &VideoWidget::on_playback_slider_pressed);
    connect(playback_slider, &QSlider::sliderReleased, this, &VideoWidget::on_playback_slider_released);
    connect(playback_slider, &QSlider::valueChanged, this, &VideoWidget::on_playback_slider_value_changed);
    connect(playback_slider, &QSlider::sliderMoved, this, &VideoWidget::on_playback_slider_moved);

    connect(frame_line_edit, &QLineEdit::editingFinished, this, &VideoWidget::frame_line_edit_finished);
}

/**
 * @brief FrameWidget::resizeEvent
 * @param event
 */
void VideoWidget::resizeEvent(QResizeEvent *event) {
    v_controller->resize();
}

void VideoWidget::stop_btn_clicked() {
    set_status_bar("Stop");
    frame_index.store(0);
    is_playing.store(false);
    on_new_frame();
}

/**
 * @brief VideoWidget::next_frame_clicked
 */
void VideoWidget::next_frame_clicked(){
    if (playback_slider->value() + 1 < m_frame_length) {
        set_status_bar("Stepping to the next frame");
        frame_index.store(playback_slider->value() + 1);
        on_new_frame();
    } else {
        set_status_bar("Already at the last frame");
    }
}

/**
 * @brief VideoWidget::prev_frame_clicked
 */
void VideoWidget::prev_frame_clicked(){
    if (playback_slider->value() - 1 > -1) {
        set_status_bar("Stepping to the previous frame");
        frame_index.store(playback_slider->value() - 1);
        on_new_frame();
    } else {
        set_status_bar("Already at the first frame");
    }
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

void VideoWidget::set_scale_factor(double scale_factor) {
    m_scale_factor = scale_factor;
    zoom_label->setText(QString::number(((int)(10000*m_scale_factor))/(double)100) +"%");
}

void VideoWidget::on_bookmark_clicked() {
    cv::Mat bookmark_frame = frame_wgt->get_mat();
    emit new_bookmark(m_vid_proj, current_frame, bookmark_frame);
}

/**
 * @brief VideoWidget::set_interval_start_clicked
 * Sets the start point of the frame interval
 */
void VideoWidget::set_interval_start_clicked() {
    m_interval.first = playback_slider->set_interval_first();
    set_status_bar("Frame interval updated: " +
                   QString().number(m_interval.first) + "-" + QString().number(m_interval.second));
    playback_slider->update();
}

/**
 * @brief VideoWidget::set_interval_end_clicked
 * Sets the end of the frame interval
*/
void VideoWidget::set_interval_end_clicked() {
    m_interval.second = playback_slider->set_interval_second();
    set_status_bar("Frame interval updated: " +
                   QString().number(m_interval.first) + "-" + QString().number(m_interval.second));
    playback_slider->update();
}

void VideoWidget::set_interval(int start, int end) {
    m_interval.first = start;
    m_interval.second = end;
    playback_slider->set_interval(start, end);
    playback_slider->update();
}

void VideoWidget::delete_interval() {
    m_interval.first = -1;
    m_interval.second = -1;
    playback_slider->clear_interval();
    playback_slider->update();
}


/**
 * @brief VideoWidget::play_btn_toggled
 * Slot function called when the play/pause button is toggled.
 * Status = true indicates that the button has been put in its play state
 * @param status
 */
void VideoWidget::play_btn_toggled(bool status) {
    is_playing.store(status);
    if (status) {
        set_status_bar("Play");
        play_btn->setIcon(QIcon("../ViAn/Icons/pause.png"));
    } else {
        play_btn->setIcon(QIcon("../ViAn/Icons/play.png"));
        set_status_bar("Pause");
    }
}

void VideoWidget::analysis_btn_clicked() {
    if (m_vid_proj != nullptr) {
        emit start_analysis(m_vid_proj);
    } else {
        emit set_status_bar("No video selected");
    }
}

void VideoWidget::tag_frame() {
    if (m_tag != nullptr) {
        Tag* tag = dynamic_cast<Tag*>(m_tag);
        tag->add_frame(playback_slider->value());
        playback_slider->set_basic_analysis(tag);
        emit set_status_bar("Tagged frame number: " + QString::number(playback_slider->value()));
        return;
    }
    emit set_status_bar("Select a tag");
}

void VideoWidget::remove_tag_frame() {
    if (m_tag != nullptr) {
        Tag* tag = dynamic_cast<Tag*>(m_tag);
        tag->remove_frame(playback_slider->value());
        playback_slider->set_basic_analysis(tag);
        emit set_status_bar("Frame untagged");
        return;
    }
    emit set_status_bar("Select a tag");
}

void VideoWidget::new_tag_clicked() {
    TagDialog* tag_dialog = new TagDialog();
    connect(tag_dialog, SIGNAL(tag_name(QString)), this, SLOT(new_tag(QString)));
    tag_dialog->exec();
}

void VideoWidget::new_tag(QString name) {
    BasicAnalysis* tag = new Tag();
    tag->m_name = name.toStdString();
    emit add_basic_analysis(m_vid_proj, tag);
}

void VideoWidget::tag_interval() {
    if (m_tag != nullptr) {
        if (m_interval.first != -1 && m_interval.second != -1 && m_interval.first <= m_interval.second) {
            m_tag->add_interval(new AnalysisInterval(m_interval.first, m_interval.second));
            playback_slider->set_basic_analysis(m_tag);
        }
    } else {
        emit set_status_bar("No tag selected");
    }
}

void VideoWidget::remove_tag_interval() {
    if (m_tag != nullptr) {
        if (m_interval.first != -1 && m_interval.second != -1 && m_interval.first <= m_interval.second) {
            m_tag->remove_interval(new AnalysisInterval(m_interval.first, m_interval.second));
            playback_slider->set_basic_analysis(m_tag);
        }
    } else {
        emit set_status_bar("No tag selected");
    }
}

void VideoWidget::set_basic_analysis(BasicAnalysis *basic_analysis) {
    m_tag = dynamic_cast<Tag*>(basic_analysis);
}

void VideoWidget::clear_tag() {
    m_tag = nullptr;
}

void VideoWidget::analysis_play_btn_toggled(bool value) {
    analysis_only = value;
}

void VideoWidget::next_poi_btn_clicked() {
    int new_frame = playback_slider->get_next_poi_start(current_frame);
    if (new_frame == current_frame) {
        emit set_status_bar("Already at last POI");
    } else {
        frame_index.store(new_frame);
        on_new_frame();
        emit set_status_bar("Jumped to next POI");
    }
}

void VideoWidget::prev_poi_btn_clicked() {
    int new_frame = playback_slider->get_prev_poi_start(current_frame);
    if (new_frame == current_frame) {
        emit set_status_bar("Already at first POI");
    } else {
        frame_index.store(new_frame);
        on_new_frame();
        emit set_status_bar("Jumped to previous POI");
    }
}

/**
 * @brief VideoWidget::zoom_out_clicked
 * zoom out button clicked.
 */
void VideoWidget::zoom_out_clicked() {
    set_status_bar("Zoom out");
}

/**
 * @brief sets the max value of the playback slider
 * @param value
 */
void VideoWidget::set_slider_max(int value) {
    playback_slider->setMaximum(value);
}

/**
 * @brief reacts to a new frame number. Updates the playback slider and current time label
 * @param frame_num
 */
void VideoWidget::on_new_frame() {
    int frame_num = frame_index.load();
    if (frame_num == m_frame_length - 1) play_btn->setChecked(false);
    if (analysis_only) {
        if (!playback_slider->is_in_POI(frame_num)) {
            if (frame_num == playback_slider->last_poi_end) {
                analysis_play_btn_toggled(false);
                analysis_play_btn->setChecked(false);
                play_btn->setChecked(false);
            } else {
                next_poi_btn_clicked();
            }
        }
    }
    if (!playback_slider->is_blocked()) {
        // Block signals to prevent value_changed signal to trigger
        playback_slider->blockSignals(true);
        playback_slider->setValue(frame_num);
        playback_slider->blockSignals(false);
    }
    current_frame = frame_num;

    set_current_time(frame_num / m_frame_rate);
    frame_line_edit->setText(QString::number(frame_num));

    playback_slider->update();
}

/**
 * @brief VideoWidget::on_playback_slider_pressed
 */
void VideoWidget::on_playback_slider_pressed() {
    timer.start();
    playback_slider->set_blocked(true);
}

void VideoWidget::on_playback_slider_released() {
    playback_slider->set_blocked(false);
    frame_index.store(playback_slider->value());
    on_new_frame();
}

void VideoWidget::on_playback_slider_value_changed() {
    frame_index.store(playback_slider->value());
    on_new_frame();
}

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

void VideoWidget::fit_clicked() {
    emit set_zoom(Utility::min_size_ratio(scroll_area->size(), current_frame_size));
}

/**
 * @brief VideoWidget::load_marked_video
 * Slot function for loading a new video
 * @param vid_proj
 */
void VideoWidget::load_marked_video(VideoProject* vid_proj, int frame) {
    if (!video_btns_enabled) enable_video_btns();

    if (m_vid_proj != vid_proj) {
        m_vid_proj = vid_proj;
        load_video(vid_proj->get_video()->file_path);
        playback_slider->setValue(frame);

        m_interval = make_pair(0,0);

        set_status_bar("Video loaded");
        play_btn->setChecked(false);
        playback_slider->set_interval(-1, -1);
    }

    if (frame > -1) {
        frame_index.store(frame);
        on_new_frame();
    }
}

void VideoWidget::enable_video_btns() {
    for (QPushButton* btn : btns) {
        btn->setEnabled(true);
    }
    playback_slider->setEnabled(true);
    frame_line_edit->setEnabled(true);
    speed_slider->setEnabled(true);
}

void VideoWidget::enable_poi_btns(bool b, bool ana_play_btn) {
    next_poi_btn->setEnabled(b);
    prev_poi_btn->setEnabled(b);

    analysis_play_btn->setEnabled(ana_play_btn);
    if (!b) {
        analysis_play_btn->setChecked(b);
        analysis_only = b;
    }
}

void VideoWidget::enable_tag_btn(bool b) {
    tag_btn->setEnabled(b);
}

void VideoWidget::on_video_info(int video_width, int video_height, int frame_rate, int last_frame){
    m_video_width = video_width;
    m_video_height = video_height;
    m_frame_rate = frame_rate;
    m_frame_length = last_frame + 1;

    current_frame_size = QSize(video_width, video_height);
    playback_slider->setMaximum(last_frame);
    set_total_time((last_frame + 1) / frame_rate);
}

void VideoWidget::on_playback_stopped(){
    play_btn->setChecked(false);
}

void VideoWidget::update_bar_pos(int change_x, int change_y) {
    h_bar->setSliderPosition(h_bar->sliderPosition() + change_x);
    v_bar->setSliderPosition(v_bar->sliderPosition() + change_y);
}

void VideoWidget::set_current_frame_size(QSize size) {
    current_frame_size = size;
}

/**
 * @brief VideoWidget::frame_line_edit_finished
 * Slot function for when the frame_line_edit is finished edited och enter is pressed
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
    }
}

