#include "videowidget.h"
#include "utility.h"
#include "drawscrollarea.h"
#include "tagdialog.h"

#include <QTime>
#include <QDebug>
#include <QShortcut>
#include <QScrollBar>

#include "Video/video_player.h"
#include "Analysis/AnalysisController.h"
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
    v_bar->hide();
    h_bar->hide();
    vertical_layout->addWidget(scroll_area);

    // End playback setup

    init_control_buttons();
    init_playback_slider();
    setLayout(vertical_layout);   

    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(m_video_player, SIGNAL(processed_image(cv::Mat)), frame_wgt, SLOT(draw_from_playback(cv::Mat)));
    connect(speed_slider, SIGNAL(valueChanged(int)), m_video_player, SLOT(set_playback_speed(int)));

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

    connect(this, SIGNAL(set_detections_on_frame(int)), frame_wgt, SLOT(set_detections_on_frame(int)));
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

    control_row->setAlignment(Qt::AlignLeft);
    video_btns->setSpacing(5);
    analysis_btns->setSpacing(5);
    other_btns->setSpacing(5);
    zoom_btns->setSpacing(5);
    control_row->setSpacing(15);

    speed_slider_layout = new QGridLayout;
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
    move_btn = new QPushButton(QIcon("../ViAn/Icons/move.png"), "", this);
    zoom_in_btn->setCheckable(true);
    move_btn->setCheckable(true);
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
    zoom_in_btn->setToolTip(tr("Zoom in"));
    zoom_out_btn->setToolTip(tr("Zoom out"));
    fit_btn->setToolTip(tr("Scale the video to screen"));
    move_btn->setToolTip(tr("Panning tool"));
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
    btns.push_back(tag_btn);
    btns.push_back(new_tag_btn);
    btns.push_back(zoom_in_btn);
    btns.push_back(zoom_out_btn);
    btns.push_back(fit_btn);
    btns.push_back(move_btn);
    
    for (QPushButton* btn : btns) {
        btn->setFixedSize(BTN_SIZE);
        btn->setEnabled(false);
    }
    next_poi_btn->setFixedSize(BTN_SIZE);
    prev_poi_btn->setFixedSize(BTN_SIZE);
    analysis_play_btn->setFixedSize(BTN_SIZE);
    enable_poi_btns(false,false);
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
    setTabOrder(fit_btn, move_btn);
}

/**
 * @brief VideoWidget::set_btn_shortcuts
 * Set shortcuts to the buttons
 */
void VideoWidget::set_btn_shortcuts() {
    play_sc = new QShortcut(Qt::Key_Space, this);
    stop_sc = new QShortcut(Qt::Key_X, this);
    next_frame_sc = new QShortcut(Qt::Key_Right, this);
    prev_frame_sc = new QShortcut(Qt::Key_Left, this);
    zoom_in_sc = new QShortcut(Qt::Key_Z, this);
    tag_sc = new QShortcut(Qt::Key_T, this);

    next_poi_sc = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right), this);
    prev_poi_sc = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left), this);
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

    connect(speed_slider, SIGNAL(valueChanged(int)), m_video_player, SLOT(set_playback_speed(int)));
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
    zoom_btns->addWidget(move_btn);
    control_row->addLayout(zoom_btns);

    vertical_layout->addLayout(control_row);
}

/**
 * @brief VideoWidget::connect_btns
 * Connect all control buttons with signals and slots
 */
void VideoWidget::connect_btns() {
    // Connect buttons, slider and actions
    connect(play_btn, &QPushButton::clicked, this, &VideoWidget::play_clicked);
    connect(play_sc, &QShortcut::activated, this, &VideoWidget::play_clicked);

    connect(stop_btn, &QPushButton::clicked, this, &VideoWidget::stop_clicked);
    connect(stop_sc, &QShortcut::activated, this, &VideoWidget::stop_clicked);

    connect(next_frame_btn, &QPushButton::clicked, this, &VideoWidget::next_frame_clicked);
    connect(next_frame_sc, &QShortcut::activated, this, &VideoWidget::next_frame_clicked);

    connect(prev_frame_btn, &QPushButton::clicked, this, &VideoWidget::prev_frame_clicked);
    connect(prev_frame_sc, &QShortcut::activated, this, &VideoWidget::prev_frame_clicked);

    connect(analysis_btn, &QPushButton::clicked, this, &VideoWidget::analysis_btn_clicked);

    connect(analysis_play_btn, &QPushButton::toggled, this, &VideoWidget::analysis_play_btn_toggled);

    connect(next_poi_btn, &QPushButton::clicked, this, &VideoWidget::next_poi_btn_clicked);
    connect(next_poi_sc, &QShortcut::activated, this, &VideoWidget::next_poi_btn_clicked);

    connect(prev_poi_btn, &QPushButton::clicked, this, &VideoWidget::prev_poi_btn_clicked);
    connect(prev_poi_sc, &QShortcut::activated, this, &VideoWidget::prev_poi_btn_clicked);

    connect(zoom_in_btn, &QPushButton::toggled, frame_wgt, &FrameWidget::toggle_zoom);
    connect(zoom_in_sc, &QShortcut::activated, zoom_in_btn, &QPushButton::toggle);

    connect(bookmark_btn, &QPushButton::clicked, this, &VideoWidget::on_bookmark_clicked);

    connect(tag_btn, &QPushButton::clicked, this, &VideoWidget::tag_frame);
    connect(tag_sc, &QShortcut::activated, this, &VideoWidget::tag_frame);

    connect(new_tag_btn, &QPushButton::clicked, this, &VideoWidget::new_tag_clicked);

    //connect(prev_frame_sc, &QShortcut::activated, this, &VideoWidget::prev_frame_clicked);

    connect(frame_wgt, &FrameWidget::trigger_zoom_out, zoom_out_btn, &QPushButton::click);
    connect(zoom_out_btn, &QPushButton::clicked, m_video_player, &video_player::zoom_out);
    //connect(prev_frame_sc, &QShortcut::activated, this, &VideoWidget::prev_frame_clicked);

    //connect(speed_slider, &QSlider::valueChanged, this, &VideoWidget::speed_slider_changed);

    connect(fit_btn, &QPushButton::clicked, m_video_player, &video_player::fit_screen);
}

/**
 * @brief Adds the video slider and playback timestamps
 */
void VideoWidget::init_playback_slider() {
    QHBoxLayout* progress_area = new QHBoxLayout();
    current_time = new QLabel("--:--");
    total_time = new QLabel("--:--");
    frame_line_edit = new QLineEdit();

    frame_line_edit->setFixedWidth(50);

    playback_slider = new AnalysisSlider(Qt::Horizontal);

    frame_line_edit->setEnabled(false);
    playback_slider->setEnabled(false);
    progress_area->addWidget(current_time);
    progress_area->addWidget(playback_slider);
    progress_area->addWidget(total_time);
    progress_area->addWidget(frame_line_edit);
    vertical_layout->addLayout(progress_area);

    // Signal/slot connect
    connect(m_video_player, SIGNAL(frame_count(int)), this, SLOT(set_slider_max(int)));
    connect(m_video_player, SIGNAL(total_time(int)), this, SLOT(set_total_time(int)));
    connect(m_video_player, SIGNAL(update_current_frame(int)), this, SLOT(on_new_frame(int)));
    connect(this, SIGNAL(set_playback_frame(int, bool)), m_video_player, SLOT(set_playback_pos(int)));
    connect(playback_slider, &QSlider::sliderPressed, this, &VideoWidget::on_playback_slider_pressed);
    connect(playback_slider, &QSlider::sliderReleased, this, &VideoWidget::on_playback_slider_released);
    connect(playback_slider, &QSlider::valueChanged, this, &VideoWidget::on_playback_slider_value_changed);
    connect(playback_slider, &QSlider::sliderMoved, this, &VideoWidget::on_playback_slider_moved);

    connect(frame_line_edit, &QLineEdit::editingFinished, this, &VideoWidget::frame_line_edit_finished);
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

void VideoWidget::on_bookmark_clicked() {
    cv::Mat bookmark_frame = frame_wgt->get_mat();
    emit new_bookmark(m_vid_proj, current_frame, bookmark_frame);
}

/**
 * @brief play/pause button click event
 */
void VideoWidget::play_clicked() {
    if (m_video_player->is_paused()) {
        play_btn->setIcon(QIcon("../ViAn/Icons/pause.png"));
        paused_wait.wakeOne();
        emit set_status_bar("Play");
    } else if (m_video_player->is_stopped()) {
        play_btn->setIcon(QIcon("../ViAn/Icons/pause.png"));
        emit set_play_video();
        m_video_player->start();
        emit set_status_bar("Play");
    } else {
        // Video is playing
        play_btn->setIcon(QIcon("../ViAn/Icons/play.png"));
        emit set_pause_video();
        emit set_status_bar("Paused");
    }
}

/**
 * @brief stop button click event
 */
void VideoWidget::stop_clicked() {
    emit set_status_bar("Stopped");
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
    int step_frame = current_frame + 1;
    if (step_frame > m_video_player->get_num_frames() - 1) {
        emit set_status_bar("Already at the end");
    } else {
        emit next_video_frame();
        emit set_status_bar("Went forward a frame to number " + QString::number(step_frame));
    }
}

/**
 * @brief previous frame button click event
 */
void VideoWidget::prev_frame_clicked() {
    int step_frame = current_frame - 1;
    if (step_frame < 0) {
        emit set_status_bar("Already at the beginning");
    } else {
        emit prev_video_frame();
        emit set_status_bar("Went backward a frame to number " + QString::number(step_frame));
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
    if (m_tag != nullptr){
        if (m_tag->type == TAG) {
            m_tag->add_frame(current_frame);
            emit set_status_bar("Tagged frame number: " + QString::number(current_frame));
            emit new_frame_tagged(m_tag);
        }
    } else {
        emit set_status_bar("Select a tag");
    }
}

void VideoWidget::new_tag_clicked() {
    TagDialog* tag_dialog = new TagDialog();
    connect(tag_dialog, SIGNAL(tag_name(QString)), this, SLOT(new_tag(QString)));
    tag_dialog->exec();
}

void VideoWidget::new_tag(QString name) {
    Analysis tag;       //TODO pointer
    tag.set_name(name.toStdString());
    tag.type = TAG;
    emit add_tag(m_vid_proj, tag);
}

void VideoWidget::set_tag(Analysis* tag) {
    m_tag = tag;
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
        emit set_status_bar("Jumped to next POI");
    }
    emit set_playback_frame(new_frame, true);
}

void VideoWidget::prev_poi_btn_clicked() {
    int new_frame = playback_slider->get_prev_poi_start(current_frame);
    if (new_frame == current_frame) {
        emit set_status_bar("Already at first POI");
    } else {
        emit set_status_bar("Jumped to previous POI");
    }
    emit set_playback_frame(new_frame, true);
}

/**
 * @brief VideoWidget::zoom_out_clicked
 * zoom out button clicked.
 */
void VideoWidget::zoom_out_clicked() {
    m_video_player->zoom_out();
    emit set_status_bar("Zoom out");
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
    if (analysis_only) {
        if (!playback_slider->is_in_POI(frame_num)) {
            if (frame_num == playback_slider->last_poi_end) {
                analysis_play_btn_toggled(false);
                analysis_play_btn->setChecked(false);
                stop_clicked();
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
    set_current_time(frame_num / m_video_player->get_frame_rate());
    frame_line_edit->setText(QString::number(current_frame));
    emit set_detections_on_frame(frame_num);
}

void VideoWidget::on_playback_slider_pressed() {
    timer.start();
    playback_slider->set_blocked(true);
    playback_slider->set_was_paused(m_video_player->is_paused());
    if (!playback_slider->get_was_paused()) emit set_pause_video();
}

void VideoWidget::on_playback_slider_released() {
    playback_slider->set_blocked(false);
    emit set_playback_frame(playback_slider->value(), true);
    if (!playback_slider->get_was_paused()) paused_wait.wakeOne();

}

void VideoWidget::on_playback_slider_value_changed() {
    if (!playback_slider->is_blocked()) {
        playback_slider->set_blocked(true);
        // Click occured
        emit set_playback_frame(playback_slider->value(), true);
        playback_slider->set_blocked(false);
    }
}

void VideoWidget::on_playback_slider_moved() {
    if (std::abs(playback_slider->value() - prev_frame_idx) % 5 == 0) {
        emit set_playback_frame(playback_slider->value(), true);
    }

    if (timer.elapsed() > 200) {
        emit set_playback_frame(playback_slider->value(), true);
        timer.restart();
    }
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
    if (!video_btns_enabled) {
        enable_video_btns();
    }
    if (m_vid_proj != vid_proj) {
        if (m_video_player->is_paused()) {
            // Playback thread sleeping, wake it
            emit set_stop_video();

            // Playback thread sleeping, wake it
            paused_wait.wakeOne();
        }

        if (m_video_player->isRunning()) {
            // Playback thread is running, stop will make it finish
            // wait until it does
            emit set_stop_video();
            m_video_player->wait();
        }
        m_vid_proj = vid_proj;
        m_video_player->load_video(m_vid_proj->get_video()->file_path, nullptr);
        emit set_status_bar("Video loaded");
        play_btn->setIcon(QIcon("../ViAn/Icons/play.png"));
        m_video_player->start();
    }
    if (frame == -1) return;
    emit set_playback_frame(frame, true);
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
        emit set_playback_frame(converted, true);
    }
}

