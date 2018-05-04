#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QCoreApplication>

#include <stack>
#include "Project/video.h"
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/core/core.hpp>
/**
 * @brief The video_sync struct
 * Video stream synchronization struct
 */
struct video_sync {
    // Lock for videostream
    std::mutex lock;
    // Conditional wait(used with lock)
    std::condition_variable con_var;
    // Frame to lock
    cv::Mat frame;
};

/**
 * @brief The video_info struct
 * Information about the video
 */
struct video_information {
    std::atomic_int total_frames{0};
    std::atomic_int total_time{0};
};

/**
 * @brief The VideoPlayer class
 * operates on an opencv capture object. Videoplayer handles reading
 * new frames as well as regulating framerate, pausing and jumping frames
 * in the video.
 */
class VideoPlayer : public QObject{
    Q_OBJECT
    cv::VideoCapture m_capture;
    std::string* m_video_path;

    std::condition_variable* m_player_con;
    std::mutex* m_player_lock;
    std::atomic_bool* m_new_frame_video;
    std::atomic_int* m_frame;
    std::atomic_int* m_speed_step;
    std::atomic_bool* m_new_frame;
    std::atomic_bool* m_new_video;
    std::atomic_bool* m_video_loaded;

    video_sync* m_v_sync;
    video_information* m_v_info;
    std::atomic_int* m_video_frames;

    // Delay time to reach the right frame rate
    int m_delay = 1;

    // Player state
    std::atomic_bool* m_is_playing;
    bool m_playback_status = false;
    int m_cur_speed_step = 1;
    double speed_multiplier = 1;
    int current_frame = -1;

    // Loaded video info
    std::atomic_int* m_video_width;
    std::atomic_int* m_video_height;
    int m_frame_rate = 0;
    int m_last_frame = 0;
public:
    explicit VideoPlayer(std::atomic<int>* frame_index, video_information* v_info, std::atomic_bool* is_playing,
                         std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                         std::atomic_bool* new_video, std::atomic_bool* new_frame_video, std::atomic_bool* video_loaded, video_sync* v_sync, std::condition_variable* player_con,
                         std::mutex* player_lock, std::string* video_path,
                         std::atomic_int* speed_step, QObject *parent = nullptr);


signals:
    void display(cv::Mat frame, int frame_index);
    void display_index();
    void video_info(int video_width, int video_height, int frame_rate, int m_last_frame);
    void playback_stopped(void);

public slots:
    void set_frame();
    void check_events(void);
private:
    void load_video_info();
    void load_video();
    bool synced_read();
    bool wait_load_read();
    void set_playback_speed(int speed_steps);

};

#endif // VIDEOPLAYER_H
