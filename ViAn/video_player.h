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

using namespace std;
using namespace cv;


class video_player : public QThread
{ Q_OBJECT
public:
    video_player(QObject* parent = 0);
    bool load_video(string filename);
    Mat play_frame(VideoCapture source_video);
    void play();
    void pause();
    bool is_paused();
signals:
    void processedImage(const QImage &image);
protected:
    void run();
    void msleep(int ms);
private:
    VideoCapture capture;
    double frame_rate;
    double speed_multiplier = 1.0;
    bool stop = false;
    Mat frame;
    Mat RGBframe;
    QImage img;
    QMutex mutex;
    QWaitCondition condition;
    bool video_paused;
};

#endif // VIDEO_PLAYER_H
