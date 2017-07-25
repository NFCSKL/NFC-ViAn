#include "videoplayer.h"
#include <QThread>
#include <QDebug>
#include <QTime>

VideoPlayer::VideoPlayer(std::atomic<int>* frame, std::atomic<bool>* is_playing, QObject *parent) : QObject(parent) {
    m_frame = frame;
    m_is_playing = is_playing;
}

/**
 * @brief VideoPlayer::on_load_video
 * Loads the video and updates member variables with video information.
 * It also emits the first frame back to the controller
 * @param video_path    :   Path to the video
 */
void VideoPlayer::on_load_video(std::string video_path){
    current_frame = -1;
    m_is_playing->store(false);
    m_frame->store(0);

    m_capture.open(video_path);
    if (!m_capture.isOpened()) return;
    load_video_info();
    emit video_info(m_video_width, m_video_height, m_frame_rate, m_last_frame);
    m_delay = 1000 / m_frame_rate;

    set_frame();
}

/**
 * @brief VideoPlayer::on_update_speed
 * Updates the playback speed.
 * @param speed_steps   :   Steps to increase/decrease
 */
void VideoPlayer::on_update_speed(int speed_steps) {
    if (speed_steps > 0) {
        speed_multiplier = 1.0 / (speed_steps * 2);
    } else if (speed_steps < 0) {
        speed_multiplier = std::abs(speed_steps) * 2;
    } else {
        speed_multiplier = 1;
    }
}

/**
 * @brief VideoPlayer::playback_loop
 * Main loop for video playback.
 * This function is executed whilst the video is playing/
 * It emits both the current frame and its index back to the controller object.
 */
void VideoPlayer::playback_loop() {
    QTime frame_rate_timer, loop_timer;
    frame_rate_timer.start();

    while (m_is_playing->load()) {
        // Handle events from controller
        QCoreApplication::processEvents();
        if (m_frame->load() != current_frame) set_frame();
        if (!m_is_playing->load()) break;

        // Make sure playback sticks to the correct frame rate
        if (frame_rate_timer.elapsed() < m_delay * speed_multiplier) continue;
        frame_rate_timer.restart();

        if (!m_capture.read(frame)) {
            m_is_playing->store(false);
            continue;
        }
        ++*m_frame;
        ++current_frame;
        display_index();
        display(frame.clone(), current_frame);
    }
    playback_stopped();
}

/**
 * @brief VideoPlayer::set_frame
 * Moves the playback to the frame indicated by the number on top of the frame_stack.
 */
void VideoPlayer::set_frame() {
    int frame_index = m_frame->load();
    if (frame_index >= 0 && frame_index <= m_last_frame) {
        m_capture.set(CV_CAP_PROP_POS_FRAMES, frame_index);
        m_capture.read(frame);

        current_frame = frame_index;
        display(frame.clone(), current_frame);
    }
}

/**
 * @brief VideoPlayer::check_events
 * Slot function for when the playback loop is not running
 */
void VideoPlayer::check_events() {
    if (m_is_playing->load()) playback_loop();
    if (m_frame->load() != current_frame) set_frame();
}

/**
 * @brief VideoPlayer::load_video_info
 * Reads video information from the capture object.
 */
void VideoPlayer::load_video_info() {
    m_video_width = m_capture.get(CV_CAP_PROP_FRAME_WIDTH);
    m_video_height = m_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    m_frame_rate = m_capture.get(CV_CAP_PROP_FPS);
    m_last_frame = m_capture.get(CV_CAP_PROP_FRAME_COUNT) - 1;
}
