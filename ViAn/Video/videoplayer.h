#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QCoreApplication>

#include <stack>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/core/core.hpp>

struct video_sync {
    std::mutex lock;
    std::condition_variable con_var;

    cv::Mat frame;
};

class VideoPlayer : public QObject{
    Q_OBJECT
    cv::VideoCapture m_capture;

    std::atomic_int* m_frame;
    std::atomic_bool* m_new_frame;
    std::atomic_bool* m_new_video;

    video_sync* m_v_sync;

    // Delay time to reach the right frame rate
    int m_delay = 0;

    // Player state
    std::atomic_bool* m_is_playing;
    bool m_video_loaded = false;
    bool m_playback_status = false;
    double speed_multiplier = 1;
    int current_frame = -1;

    // Loaded video info
    std::atomic_int* m_video_width;
    std::atomic_int* m_video_height;
    int m_frame_rate = 0;
    int m_last_frame = 0;
public:
    explicit VideoPlayer(std::atomic<int>* frame_index, std::atomic<bool>* is_playing,
                         std::atomic_bool* new_frame, std::atomic_int* width, std::atomic_int* height,
                         std::atomic_bool* new_video, video_sync* v_sync, QObject *parent = nullptr);

signals:
    void display(cv::Mat frame, int frame_index);
    void display_index();
    void video_info(int video_width, int video_height, int frame_rate, int m_last_frame);
    void playback_stopped(void);

public slots:
    void on_load_video(std::string);
    void on_update_speed(int speed_steps);
    void set_frame();
    void check_events(void);

private:
    void playback_loop();
    void load_video_info();
    bool synced_read();
};

#endif // VIDEOPLAYER_H
