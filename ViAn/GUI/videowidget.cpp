#include "videowidget.h"
#include "utility.h"
#include "drawscrollarea.h"

#include <QTime>
#include <QDebug>
#include <QShortcut>
#include <QScrollBar>

#include "Video/video_player.h"
#include "Video/videoplayer.h"
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent), scroll_area(new DrawScrollArea) {
    // Init video player
    m_video_player = new video_player(&mutex, &paused_wait);

    //Setup playback area
    vertical_layout = new QVBoxLayout;
    frame_wgt = new FrameWidget();
    frame_wgt->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    scroll_area->setBackgroundRole(QPalette::Dark);
    scroll_area->setWidget(frame_wgt);
    scroll_area->setFrameStyle(0);
    v_bar = scroll_area->verticalScrollBar();
    h_bar = scroll_area->horizontalScrollBar();
    v_bar->setSingleStep(1);
    vertical_layout->addWidget(scroll_area);

    // End playback setup

    init_control_buttons();
    init_playback_slider();
    setLayout(vertical_layout);   

    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(m_video_player, SIGNAL(processed_image(cv::Mat)), frame_wgt, SLOT(draw_from_playback(cv::Mat)));
    connect(speed_slider, SIGNAL(valueChanged(int)), m_video_player, SLOT(set_playback_speed(int)));

    connect(frame_wgt, SIGNAL(zoom_factor(double)), m_video_player, SLOT(update_zoom(double)));
    connect(frame_wgt, SIGNAL(zoom_points(QPoint, QPoint)), m_video_player, SLOT(set_zoom_rect(QPoint, QPoint)));
    connect(frame_wgt, SIGNAL(moved_xy(int,int)), this, SLOT(update_bar_pos(int,int)));
    connect(frame_wgt, SIGNAL(moved_xy(int,int)), m_video_player, SLOT(on_move_zoom_rect(int,int)));

    connect(scroll_area, SIGNAL(new_size(QSize)), frame_wgt, SLOT(set_scroll_area_size(QSize)));
    connect(scroll_area, SIGNAL(new_size(QSize)), m_video_player, SLOT(set_viewport_size(QSize)));

    connect(m_video_player, SIGNAL(capture_frame_size(QSize)), this, SLOT(set_current_frame_size(QSize)));
    connect(this, &VideoWidget::set_play_video, m_video_player, &video_player::on_play_video);
    connect(this, &VideoWidget::set_pause_video, m_video_player, &video_player::on_pause_video);
    connect(this, &VideoWidget::set_stop_video, m_video_player, &video_player::on_stop_video);
    connect(this, &VideoWidget::next_video_frame, m_video_player, &video_player::next_frame);
    connect(this, &VideoWidget::prev_video_frame, m_video_player, &video_player::previous_frame);
    connect(this, SIGNAL(zoom_out(double)), m_video_player, SLOT(update_zoom(double)));
    connect(this, SIGNAL(set_zoom(double)), m_video_player, SLOT(set_zoom(double)));
}

/**
 * @brief Adds all the video control buttons to the video widget
 */
void VideoWidget::init_control_buttons() {
    QHBoxLayout* controll_row = new QHBoxLayout;    // Container for all button areas
    QHBoxLayout* other_btns = new QHBoxLayout;      // Bookmark, tag
    QHBoxLayout* video_btns = new QHBoxLayout;      // Play, pause etc
    QHBoxLayout* analysis_btns = new QHBoxLayout;   // Buttons for starting analysis and jumping between pois
    QHBoxLayout* zoom_btns = new QHBoxLayout;       // Zoom buttons
    controll_row->setAlignment(Qt::AlignLeft);
    video_btns->setSpacing(5);
    other_btns->setSpacing(5);
    analysis_btns->setSpacing(5);
    zoom_btns->setSpacing(5);
    controll_row->setSpacing(15);

    std::vector<QPushButton*> btns;

    play_btn = new QPushButton(QIcon("../ViAn/Icons/play.png"), "", this);
    QPushButton* stop_btn = new QPushButton(QIcon("../ViAn/Icons/stop.png"), "", this);
    QPushButton* next_frame_btn = new QPushButton(QIcon("../ViAn/Icons/next_frame.png"), "", this);
    QPushButton* prev_frame_btn = new QPushButton(QIcon("../ViAn/Icons/prev_frame.png"), "", this);
    QPushButton* next_poi_btn = new QPushButton(QIcon("../ViAn/Icons/next_poi.png"), "", this);
    QPushButton* prev_poi_btn = new QPushButton(QIcon("../ViAn/Icons/prev_poi.png"), "", this);
    QPushButton* bookmark_btn = new QPushButton(QIcon("../ViAn/Icons/bookmark.png"), "", this);
    QPushButton* analysis_btn = new QPushButton(QIcon("../ViAn/Icons/analysis.png"), "", this);
    QPushButton* tag_btn = new QPushButton(QIcon("../ViAn/Icons/tag.png"), "", this);
    zoom_in_btn = new QPushButton(QIcon("../ViAn/Icons/zoom_in.png"), "", this);
    QPushButton* zoom_out_btn = new QPushButton(QIcon("../ViAn/Icons/zoom_out.png"), "", this);
    fit_btn = new QPushButton(QIcon("../ViAn/Icons/fit_screen.png"), "", this);
    QPushButton* move_btn = new QPushButton(QIcon("../ViAn/Icons/move.png"), "", this);

    zoom_in_btn->setCheckable(true);
    move_btn->setCheckable(true);

    play_btn->setToolTip(tr("Play video"));
    stop_btn->setToolTip(tr("Stop video"));
    next_frame_btn->setToolTip(tr("Next frame"));
    prev_frame_btn->setToolTip(tr("Previous frame"));
    bookmark_btn->setToolTip(tr("Bookmark the current frame"));
    tag_btn->setToolTip(tr("Tag the current frame"));
    next_poi_btn->setToolTip(tr("Next POI"));
    analysis_btn->setToolTip(tr("Analysis"));
    prev_poi_btn->setToolTip(tr("Previous POI"));

    btns.push_back(play_btn);
    btns.push_back(stop_btn);
    btns.push_back(next_frame_btn);
    btns.push_back(prev_frame_btn);
    btns.push_back(next_poi_btn);
    btns.push_back(prev_poi_btn);
    btns.push_back(bookmark_btn);
    btns.push_back(analysis_btn);
    btns.push_back(tag_btn);
    btns.push_back(zoom_in_btn);
    btns.push_back(zoom_out_btn);
    btns.push_back(fit_btn);
    btns.push_back(move_btn);

    // Create and add speed adjustment slider
    speed_slider = new QSlider(Qt::Horizontal);
    speed_slider->setRange(-4,4);
    speed_slider->setMaximumWidth(90);
    speed_slider->setTickPosition(QSlider::TicksBelow);
    speed_slider->setToolTip(tr("Adjust playback speed"));

    for (QPushButton* btn : btns) {
        btn->setFixedSize(BTN_SIZE);
    }

    video_btns->addWidget(prev_frame_btn);
    video_btns->addWidget(play_btn);
    video_btns->addWidget(next_frame_btn);
    video_btns->addWidget(stop_btn);
    video_btns->addWidget(speed_slider);
    controll_row->addLayout(video_btns);

    analysis_btns->addWidget(prev_poi_btn);
    analysis_btns->addWidget(analysis_btn);
    analysis_btns->addWidget(next_poi_btn);
    controll_row->addLayout(analysis_btns);

    other_btns->addWidget(bookmark_btn);
    other_btns->addWidget(tag_btn);
    controll_row->addLayout(other_btns);

    zoom_btns->addWidget(zoom_in_btn);
    zoom_btns->addWidget(zoom_out_btn);
    zoom_btns->addWidget(fit_btn);
    zoom_btns->addWidget(move_btn);
    controll_row->addLayout(zoom_btns);

    vertical_layout->addLayout(controll_row);

    QShortcut* play_sc = new QShortcut(Qt::Key_Space, this);
    QShortcut* stop_sc = new QShortcut(Qt::Key_X, this);
    QShortcut* zoom_in_sc = new QShortcut(Qt::Key_Z, this);
    QShortcut* next_frame_sc = new QShortcut(Qt::Key_Right, this);
    QShortcut* prev_frame_sc = new QShortcut(Qt::Key_Left, this);
    //QShortcut* next_poi_sc = new QShortcut(Qt::Key_Space, this);
    //QShortcut* prev_poi_sc = new QShortcut(Qt::Key_Space, this);

    // Connect buttons, slider and actions
    connect(play_btn, &QPushButton::clicked, this, &VideoWidget::play_clicked);
    connect(play_sc, &QShortcut::activated, this, &VideoWidget::play_clicked);

    connect(stop_btn, &QPushButton::clicked, this, &VideoWidget::stop_clicked);
    connect(stop_sc, &QShortcut::activated, this, &VideoWidget::stop_clicked);

    connect(next_frame_btn, &QPushButton::clicked, this, &VideoWidget::next_frame_clicked);
    connect(next_frame_sc, &QShortcut::activated, this, &VideoWidget::next_frame_clicked);

    connect(prev_frame_btn, &QPushButton::clicked, this, &VideoWidget::prev_frame_clicked);
    connect(prev_frame_sc, &QShortcut::activated, this, &VideoWidget::prev_frame_clicked);

    connect(zoom_in_btn, &QPushButton::toggled, frame_wgt, &FrameWidget::toggle_zoom);
    connect(zoom_in_sc, &QShortcut::activated, zoom_in_btn, &QPushButton::toggle);
    //connect(prev_frame_sc, &QShortcut::activated, this, &VideoWidget::prev_frame_clicked);

    connect(zoom_out_btn, &QPushButton::clicked, m_video_player, &video_player::zoom_out);
    //connect(prev_frame_sc, &QShortcut::activated, this, &VideoWidget::prev_frame_clicked);

    //connect(speed_slider, &QSlider::valueChanged, this, &VideoWidget::speed_slider_changed);

    //
    connect(fit_btn, &QPushButton::clicked, m_video_player, &video_player::fit_screen);
}

/**
 * @brief Adds the video slider and playback timestamps
 */
void VideoWidget::init_playback_slider() {
    QHBoxLayout* progress_area = new QHBoxLayout();
    current_time = new QLabel("--:--");
    total_time = new QLabel("--:--");
    playback_slider = new QSlider(Qt::Horizontal);
    progress_area->addWidget(current_time);
    progress_area->addWidget(playback_slider);
    progress_area->addWidget(total_time);
    vertical_layout->addLayout(progress_area);

    // Signal/slot connect
    connect(m_video_player, SIGNAL(frame_count(int)), this, SLOT(set_slider_max(int)));
    connect(m_video_player, SIGNAL(total_time(int)), this, SLOT(set_total_time(int)));
    connect(m_video_player, SIGNAL(update_current_frame(int)), this, SLOT(on_new_frame(int)));
    connect(this, SIGNAL(set_playback_frame(int, bool)), m_video_player, SLOT(on_set_playback_frame(int)));
    connect(playback_slider, &QSlider::sliderPressed, this, on_playback_slider_pressed);
    connect(playback_slider, &QSlider::sliderReleased, this, on_playback_slider_released);
    connect(playback_slider, &QSlider::valueChanged, this, on_playback_slider_value_changed);
    connect(playback_slider, &QSlider::sliderMoved, this, on_playback_slider_moved);
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
 * @brief play/pause button click event
 */
void VideoWidget::play_clicked() {
    double horizontal_step_size = h_bar->maximum() / 1280;
    qDebug() << horizontal_step_size;
    if (m_video_player->is_paused()) {
        play_btn->setIcon(QIcon("../ViAn/Icons/pause.png"));
        paused_wait.wakeOne();
        // TODO status bar
    } else if (m_video_player->is_stopped()) {
         play_btn->setIcon(QIcon("../ViAn/Icons/pause.png"));
         emit set_play_video();
         m_video_player->start();
    } else {
        // Video is playing
        play_btn->setIcon(QIcon("../ViAn/Icons/play.png"));
        emit set_pause_video();
        // status bar
    }
}

/**
 * @brief stop button click event
 */
void VideoWidget::stop_clicked() {
    //set_status_bar("Stopped");
    if (m_video_player->is_playing()) {
         play_btn->setIcon(QIcon("../ViAn/Icons/play.png"));
    } else if (m_video_player->is_paused()) {
        paused_wait.wakeOne();
    } else if (m_video_player->is_stopped()) {
        return;
    }
    emit set_stop_video();
}

/**
 * @brief next frame button click event
 */
void VideoWidget::next_frame_clicked() {
    if (m_video_player->is_paused()) {
        emit next_video_frame();
        int frame = m_video_player->get_current_frame_num();
        //set_status_bar("Went forward a frame to number " + std::to_string(frame));
    } else {
        //set_status_bar("Needs to be paused");
    }
}

/**
 * @brief previous frame button click event
 */
void VideoWidget::prev_frame_clicked() {
    if (m_video_player->is_paused()) {
        emit prev_video_frame();
        int frame = m_video_player->get_current_frame_num();
        //set_status_bar("Went back a frame to number " + std::to_string(frame));
    } else {
        //set_status_bar("Video needs to be paused");
    }
}

/**
 * @brief VideoWidget::zoom_out_clicked
 * zoom out button clicked.
 */
void VideoWidget::zoom_out_clicked() {
    emit zoom_out(0.25);
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
void VideoWidget::on_new_frame(int frame_num) {
    if (!slider_is_blocked)
        playback_slider->setValue(frame_num);
    set_current_time(frame_num / m_video_player->get_frame_rate());
}

void VideoWidget::on_playback_slider_pressed() {
    slider_is_blocked = true;
}

void VideoWidget::on_playback_slider_released() {
    emit set_playback_frame(playback_slider->value(), true);
    slider_is_blocked = false;
}

void VideoWidget::on_playback_slider_value_changed() {
    if (slider_is_blocked) {
        //qDebug() << "emit slider value";
        //emit set_playback_frame(slider_pos, true);
    } else {
        //qDebug();
    }
}

void VideoWidget::on_playback_slider_moved() {
    qDebug() << "moved";
}

void VideoWidget::fit_clicked() {
    emit set_zoom(Utility::min_size_ratio(scroll_area->size(), current_frame_size));
}

/**
 * @brief VideoWidget::load_marked_video
 * Slot function for loading a new video
 * @param vid_proj
 */
void VideoWidget::load_marked_video(VideoProject* vid_proj) {
    m_vid_proj = vid_proj;
    if (m_video_player->is_paused()) {
        // Playback thread sleeping, wake it
        emit set_stop_video();
        paused_wait.wakeOne();
    }

    if (m_video_player->isRunning()) {
        // Playback thread is running, stop will make it finish
        // wait until it does
        emit set_stop_video();
        m_video_player->wait();
    }
    m_video_player->load_video(m_vid_proj->get_video()->file_path, nullptr);
    m_video_player->start();
}

void VideoWidget::update_bar_pos(int change_x, int change_y) {
    h_bar->setSliderPosition(h_bar->sliderPosition() + change_x);
    v_bar->setSliderPosition(v_bar->sliderPosition() + change_y);
}

void VideoWidget::set_current_frame_size(QSize size) {
    current_frame_size = size;
}


