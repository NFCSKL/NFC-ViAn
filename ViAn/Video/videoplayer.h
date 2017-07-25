#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QCoreApplication>

#include <stack>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/core/core.hpp>

class VideoPlayer : public QObject
{
    Q_OBJECT
    cv::VideoCapture m_capture;
    cv::Mat frame;

    std::atomic<int>* m_frame;


    int m_delay = 0;    // Delay time to reach the right frame rate

    // Player state
    std::atomic<bool>* m_is_playing;
    bool m_video_loaded = false;
    bool m_playback_status = false;
    double speed_multiplier = 1;
    int current_frame = -1;
    std::string m_vid_path = "D:/Testdata/Sequence 01.mp4";

    // Loaded video info
    int m_video_width = 0;
    int m_video_height = 0;
    int m_frame_rate = 0;
    int m_last_frame = 0;
public:
    explicit VideoPlayer(std::atomic<int>* frame, std::atomic<bool>* is_playing, QObject *parent = nullptr);

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
};

#endif // VIDEOPLAYER_H
