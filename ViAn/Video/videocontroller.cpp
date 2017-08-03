#include "videocontroller.h"


#include <QDebug>

VideoController::VideoController(std::atomic<int>* frame_index, std::atomic_bool* is_playing,
                                 std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                                 std::atomic_bool* new_video, std::atomic_bool* new_frame_video,video_sync* v_sync, std::condition_variable* player_con,
                                 std::mutex* player_lock, std::string* video_path, std::atomic_int* speed){

    m_frame = frame_index;
    m_is_playing = is_playing;
    m_width = width;
    m_height = height;
    m_new_video = new_video;
    m_new_frame_video = new_frame_video;
    m_v_sync = v_sync;
    m_new_frame = new_frame;

    m_player_con = player_con;
    m_player_lock = player_lock;
    m_video_path = video_path;
    m_speed = speed;

}

void VideoController::run() {
    VideoPlayer* v_player = new VideoPlayer(m_frame, m_is_playing, m_new_frame,
                                            m_width, m_height, m_new_video, m_new_frame_video,
                                            m_v_sync, m_player_con, m_player_lock, m_video_path,
                                            m_speed);

    // FROM PLAYER
    connect(v_player, &VideoPlayer::display_index, this, &VideoController::display_index);
    connect(v_player, &VideoPlayer::video_info, this, &VideoController::video_info);
    connect(v_player, &VideoPlayer::playback_stopped, this, &VideoController::playback_stopped);

    v_player->check_events();
    exec();
    delete v_player;

}
