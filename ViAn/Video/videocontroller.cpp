#include "videocontroller.h"
#include "videoplayer.h"

#include <QDebug>

VideoController::VideoController(std::atomic<int>* frame, std::atomic<bool>* is_playing){
    m_frame = frame;
    m_is_playing = is_playing;
}

void VideoController::run() {
    VideoPlayer* v_player = new VideoPlayer(m_frame, m_is_playing);
    FrameProcessor* f_processor = new FrameProcessor();
    frame_timer = new QTimer();

    // TO PLAYER
    connect(this, &VideoController::load_video, v_player, &VideoPlayer::on_load_video);
        // CONTROL SIGNALS
    connect(this, &VideoController::update_speed, v_player, &VideoPlayer::on_update_speed);
    connect(frame_timer, SIGNAL(timeout()), v_player, SLOT(check_events()));

    // FROM PLAYER
    connect(v_player, &VideoPlayer::display, f_processor, &FrameProcessor::on_new_frame);
    connect(v_player, &VideoPlayer::display_index, this, &VideoController::display_index);
    connect(v_player, &VideoPlayer::video_info, this, &VideoController::video_info);
    connect(v_player, &VideoPlayer::video_info, f_processor, &FrameProcessor::on_video_info);
    connect(v_player, &VideoPlayer::playback_stopped, this, &VideoController::playback_stopped);

    // TO PROCESSOR
        // ZOOM
    connect(this, &VideoController::set_zoom_rect, f_processor, &FrameProcessor::on_set_zoom_rect);
    connect(this, &VideoController::set_draw_area_size, f_processor, &FrameProcessor::on_set_draw_area_size);
    connect(this, &VideoController::zoom_out, f_processor, &FrameProcessor::on_zoom_out);
    connect(this, &VideoController::fit_screen, f_processor, &FrameProcessor::on_fit_screen);
    connect(this, &VideoController::original_size, f_processor, &FrameProcessor::on_original_size);
    connect(this, &VideoController::move_zoom_rect, f_processor, &FrameProcessor::on_move_zoom_rect);
    connect(this, &VideoController::resize, f_processor, &FrameProcessor::on_resize);
        // ROTATION
    connect(this, &VideoController::rotate_right, f_processor, &FrameProcessor::on_rotate_right);
    connect(this, &VideoController::rotate_left, f_processor, &FrameProcessor::on_rotate_left);
        // BRIGHTNESS, CONTRAST
    connect(this, &VideoController::set_bright_cont, f_processor, &FrameProcessor::on_set_bright_cont);

    // FROM PROCESSOR
    connect(f_processor, &FrameProcessor::done_processing, this, &VideoController::display_image);
    connect(f_processor, &FrameProcessor::set_scale_factor, this, &VideoController::scale_factor);
    connect(f_processor, &FrameProcessor::set_anchor, this, &VideoController::anchor);

    frame_timer->start(5);
    exec();
    delete v_player;
    delete f_processor;
    delete frame_timer;
}
