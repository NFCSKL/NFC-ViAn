#include "videoplayer.h"
#include <QThread>
#include <QDebug>
#include <QTime>

VideoPlayer::VideoPlayer(std::atomic<int>* frame_index, std::atomic_bool *is_playing,
                         std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                         std::atomic_bool* new_video, std::atomic_bool *new_frame_video, std::atomic_bool *video_loaded, video_sync* v_sync, std::condition_variable* player_con,
                         std::mutex* player_lock, std::string* video_path,
                         std::atomic_int* speed_step, QObject *parent) : QObject(parent) {


    m_frame = frame_index;
    m_is_playing = is_playing;
    m_new_frame = new_frame;

    m_video_width = width;
    m_video_height = height;
    m_new_video = new_video;
    m_new_frame_video = new_frame_video;
    m_video_loaded = video_loaded;
    m_v_sync = v_sync;
    m_player_con = player_con;
    m_player_lock = player_lock;

    m_video_path = video_path;
    m_speed_step = speed_step;

}

VideoPlayer::~VideoPlayer() {
    loop = false;
    m_player_con->notify_all();
}

/**
 * @brief VideoPlayer::on_load_video
 * Loads the video and updates member variables with video information.
 * It also emits the first frame back to the controller
 * @param video_path    :   Path to the video
 */

void VideoPlayer::load_video() {
    m_video_loaded->store(true);
    current_frame = -1;
    m_is_playing->store(false);
    m_capture.open(*m_video_path);
    if (!m_capture.isOpened()) return;
    load_video_info();
    emit video_info(m_video_width->load(), m_video_height->load(), m_frame_rate, m_last_frame);
    m_delay = 1000 / m_frame_rate;

    m_new_video->store(false);
    m_new_frame_video->store(true);
}

/**
 * @brief VideoPlayer::on_update_speed
 * Updates the playback speed.
 * @param speed_steps   :   Steps to increase/decrease
 */
void VideoPlayer::set_playback_speed(int speed_steps) {
    if (speed_steps > 0) {
        speed_multiplier = 1.0 / (speed_steps * 2);
    } else if (speed_steps < 0) {
        speed_multiplier = std::abs(speed_steps) * 2;
    } else {
        speed_multiplier = 1;
    }
}

/**
 * @brief VideoPlayer::set_frame
 * Moves the playback to the frame indicated by the number on top of the frame_stack.
 */
void VideoPlayer::set_frame() {
    int frame_index = m_frame->load();
    if (frame_index >= 0 && frame_index <= m_last_frame) {
        m_capture.set(CV_CAP_PROP_POS_FRAMES, frame_index);
        current_frame = frame_index;
        synced_read();
    }
}

/**
 * @brief VideoPlayer::check_events
 * Main loop for video playback.
 */
void VideoPlayer::check_events() {
    std::chrono::duration<double> elapsed{0};
    while (loop) {
        std::unique_lock<std::mutex> lk(*m_player_lock);
        auto now = std::chrono::system_clock::now();
        auto delay = std::chrono::milliseconds{static_cast<int>(m_delay * speed_multiplier)};
        if (m_player_con->wait_until(lk, now + delay - elapsed, [&](){return !loop || m_new_video->load() || (current_frame != m_frame->load() && m_video_loaded->load());})) {
            // Notified from the VideoWidget
            if (m_new_video->load()) {
                wait_load_read();
            } else if (current_frame != m_frame->load() && m_video_loaded->load()) {
                set_frame();
            }
        } else {
            // Timer condition triggered. Update playback speed if nessecary and read new frame
            int speed = m_speed_step->load();
            if (speed != m_cur_speed_step) {
                set_playback_speed(speed);
            }

            // Timer condition triggered. Read new frame
            if (m_is_playing->load()) {
                std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
                if (!synced_read()) continue;
                ++*m_frame;
                ++current_frame;
                display_index();
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                elapsed = end - start;
            }
        }
        lk.unlock();
    }
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
            playback_stopped();

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


bool VideoPlayer::wait_load_read(){
    // Wait for processing thread to finish processing new frame
    {
        std::unique_lock<std::mutex> lk(m_v_sync->lock);
        m_v_sync->con_var.wait(lk, [&]{return !m_new_frame->load();});
    }
    // Read new frame and notify processing thread
    {
        std::lock_guard<std::mutex> lk(m_v_sync->lock);
        load_video();
        if (!m_capture.read(m_v_sync->frame)) {
            m_is_playing->store(false);
            playback_stopped();

            return false;
        }
        m_new_frame->store(true);
    }
    m_v_sync->con_var.notify_one();
    return true;
}
