#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"


#include <QThread>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include <chrono>

using namespace std;


class video_player : public QThread
{ Q_OBJECT
public:
    video_player(QObject* parent = 0);
    bool load_video(string filename);
    void play_pause();
    bool is_paused();
    bool is_stopped();
    int get_num_frames();
    void set_frame_width(int new_value);
    void set_frame_height(int new_value);
    void set_playback_frame(int frame_num);
    void stop_video();

    friend class test_video_player;

signals:
    void processedImage(const QImage &image);
    void currentFrame(const int frame);
protected:
    void run() override;
    void msleep(int ms);
private:
    unsigned int current_frame = 0;
    cv::VideoCapture capture;
    double frame_rate;
    double speed_multiplier = 1.0;
    bool stop = false;
    cv::Mat frame;
    cv::Mat RGBframe;
    QImage img;
    QWaitCondition condition;
    bool video_paused;
    int num_frames;
    unsigned int frame_width;
    unsigned int frame_height;
};

#endif // VIDEO_PLAYER_H
