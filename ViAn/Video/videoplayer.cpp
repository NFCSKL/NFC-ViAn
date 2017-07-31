#include "videoplayer.h"
#include <QThread>
#include <QDebug>
#include <QTime>

VideoPlayer::VideoPlayer(std::atomic<int>* frame_index, std::atomic<bool>* is_playing,
                         std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                         std::atomic_bool* new_video, video_sync* v_sync, QObject *parent) : QObject(parent) {
    m_frame = frame_index;
    m_is_playing = is_playing;
    m_new_frame = new_frame;

    m_video_width = width;
    m_video_height = height;
    m_new_video = new_video;

    m_v_sync = v_sync;
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
    emit video_info(m_video_width->load(), m_video_height->load(), m_frame_rate, m_last_frame);
    m_delay = 1000 / m_frame_rate;

    m_new_video->store(true);
    m_v_sync->con_var.notify_all();


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
    QTime frame_rate_timer;
    frame_rate_timer.start();

    while (m_is_playing->load()) {
        // Handle events from controller
        QCoreApplication::processEvents();
        if (m_frame->load() != current_frame) set_frame();
        if (!m_is_playing->load()) break;

        // Make sure playback sticks to the correct frame rate
        if (frame_rate_timer.elapsed() < m_delay * speed_multiplier) {
            QThread::msleep(1); // Reduces busy waiting
            continue;
        }
        frame_rate_timer.restart();

        if (!synced_read()) break;

        ++*m_frame;
        ++current_frame;
        display_index();
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
        synced_read();
        current_frame = frame_index;
    }
}

/**
 * @brief VideoPlayer::check_events
 * Slot function for when the playback loop is not running1
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
    m_video_width->store(m_capture.get(CV_CAP_PROP_FRAME_WIDTH));
    m_video_height->store(m_capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    m_frame_rate = m_capture.get(CV_CAP_PROP_FPS);
    m_last_frame = m_capture.get(CV_CAP_PROP_FRAME_COUNT) - 1;
}

bool VideoPlayer::synced_read(){
    // Read new frame and notify processing thread
   {
        std::lock_guard<std::mutex> lk(m_v_sync->lock);
        if (!m_capture.read(m_v_sync->frame)) {
            m_is_playing->store(false);
            return false;
        }
        m_new_frame->store(true);
    }
    m_v_sync->con_var.notify_one();

    // Wait for processing thread to finish processing new frame
    {
        std::unique_lock<std::mutex> lk(m_v_sync->lock);
        m_v_sync->con_var.wait(lk, [&]{return !m_new_frame->load();});
    }
    return true;
}
