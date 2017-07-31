#include "videocontroller.h"


#include <QDebug>

VideoController::VideoController(std::atomic<int>* frame_index, std::atomic<bool>* is_playing,
                                 std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                                 std::atomic_bool* new_video, video_sync* v_sync){
    m_frame = frame_index;
    m_is_playing = is_playing;
    m_width = width;
    m_height = height;
    m_new_video = new_video;
    m_v_sync = v_sync;
    m_new_frame = new_frame;
}

void VideoController::run() {
    VideoPlayer* v_player = new VideoPlayer(m_frame, m_is_playing, m_new_frame,
                                            m_width, m_height, m_new_video,
                                            m_v_sync);
    frame_timer = new QTimer();

    // TO PLAYER
    connect(this, &VideoController::load_video, v_player, &VideoPlayer::on_load_video);
        // CONTROL SIGNALS
    connect(this, &VideoController::update_speed, v_player, &VideoPlayer::on_update_speed);
    connect(frame_timer, SIGNAL(timeout()), v_player, SLOT(check_events()));

    // FROM PLAYER
    connect(v_player, &VideoPlayer::display_index, this, &VideoController::display_index);
    connect(v_player, &VideoPlayer::video_info, this, &VideoController::video_info);
    connect(v_player, &VideoPlayer::playback_stopped, this, &VideoController::playback_stopped);


    frame_timer->start(5);
    exec();
    delete v_player;
    delete frame_timer;
}
