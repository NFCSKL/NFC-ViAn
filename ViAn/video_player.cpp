#include "video_player.h"
#include <string>
#include <iostream>
#include "mainwindow.h"
#include <thread>
#include <QWaitCondition>


using namespace std;
using namespace cv;

video_player::video_player(QObject* parent) : QThread(parent) {}


// This method loads a video from file.
bool video_player::load_video(string filename) {
    capture.open(filename);
    if (capture.isOpened()) {
        frame_rate = capture.get(CV_CAP_PROP_FPS);
        video_paused = false;
        start();
        return true;
    }
    else
        cout << "Could not load file \"" << filename << "\"" << endl;
        return false;
}

void video_player::play() {
    video_paused = false;
    //condition.wakeAll();
}

void video_player::pause() {
    video_paused = true;
}

/* This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    video_paused = false;
    stop = false;
    int delay = (1000/frame_rate);
    cout << "Current frame rate: " << frame_rate << ", actual frame rate: " << capture.get(CV_CAP_PROP_FPS) << endl;

    capture.set(CV_CAP_PROP_POS_FRAMES,current_frame);
    //Debug print
    cout << "stop: " << stop << ", video_paused: " << video_paused << ", capture.read(frame): " << capture.read(frame) << endl;


    //Runs the video as long as it is not paused, stopped or ended.
    while(!stop && !video_paused && capture.read(frame)){

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
    cout << "Exiting main video loop." << endl;
    //Saves the current frame of the video if the video is paused.
    if (video_paused) {
        current_frame = capture.get(CV_CAP_PROP_POS_FRAMES);
    }
}

//This method is used to make the video player thread sleep the correct amount of time between fetching frames from source video.
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

//Returns a boolean value representing whether the currently played video is paused.
bool video_player::is_paused() {
    return video_paused;
}

