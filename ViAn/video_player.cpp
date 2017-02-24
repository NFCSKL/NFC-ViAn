#include "video_player.h"
#include <string>
#include <iostream>
#include "mainwindow.h"
#include <thread>
#include <QWaitCondition>


using namespace std;
using namespace cv;

video_player::video_player(QObject* parent) : QThread(parent) {

}

bool video_player::load_video(string filename) {
    capture.open(filename);
    if (capture.isOpened()) {
        num_frames = capture.get(CAP_PROP_FRAME_COUNT);
        frame_rate = (int) capture.get(CV_CAP_PROP_FPS);
        video_paused = false;
        start();
        return true;
    }
    else
        return false;
}

void video_player::play() {
    video_paused = false;
    condition.wakeAll();
}

void video_player::pause() {
    video_paused = true;
}

void video_player::run() {
    int delay = (1000/frame_rate);
    while(!stop){
        if (video_paused) {
            condition.wait(&mutex);
        }

        emit currentFrame(capture.get(CAP_PROP_POS_FRAMES));

        if (!capture.read(frame)) {
            stop = true;
        }
        if (frame.channels()== 3) {
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        } else {
            img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        emit processedImage(img);
        this->msleep(delay);
    }
}

void video_player::msleep(int delay) {
    std::chrono::milliseconds dura(delay);
    std::this_thread::sleep_for( dura );
}

Mat video_player::play_frame(VideoCapture source_video) {
    Mat frame;
    source_video >> frame;
    return frame;
    double fps = source_video.get(CV_CAP_PROP_FPS);
}

bool video_player::is_paused() {
    return video_paused;
}

int video_player::get_num_frames() {
    return num_frames;
}

