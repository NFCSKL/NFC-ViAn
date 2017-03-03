#include "video_player.h"
#include <string>
#include <iostream>
#include "GUI/mainwindow.h"
#include <thread>
#include <QWaitCondition>


using namespace std;
using namespace cv;

video_player::video_player(QObject* parent) : QThread(parent) {
    video_paused = false;
}


//
/**
 * @brief video_player::load_video
 * This method loads a video from file.
 * @param filename
 * @return whether video is loaded
 */
bool video_player::load_video(string filename) {
    capture.open(filename);

    if (capture.isOpened()) {
        frame_rate = capture.get(CV_CAP_PROP_FPS);
        num_frames = capture.get(CAP_PROP_FRAME_COUNT);
        video_paused = false;
        start();
        return true;
    }
    else
        cout << "Could not load file \"" << filename << "\"" << endl;
        return false;
}

/**
 * @brief video_player::play
 * Toggles the video_paused boolean
 */
void video_player::play_pause() {
    video_paused = !video_paused;
}

/**
 * @brief video_player::run
 * This function is called whenever the thread is started or put out of sleep.
 * It houses the main loop for fetching frames from the currently played
 * video file and sending them to the GUI.
 */
void video_player::run()  {
    stop = false;
    int delay = (1000/frame_rate);

    capture.set(CV_CAP_PROP_POS_FRAMES,current_frame);

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
        emit currentFrame(capture.get(CV_CAP_PROP_POS_FRAMES));
        this->msleep(delay);

    }
    //Saves the current frame of the video if the video is paused.
    if (video_paused) {
        current_frame = capture.get(CV_CAP_PROP_POS_FRAMES);
    }
}


/**
 * @brief video_player::msleep
 * This method is used to make the video player thread sleep the correct amount
 * of time between fetching frames from source video.
 * @param delay
 */
void video_player::msleep(int delay) {
    std::chrono::milliseconds dura((int)(delay*this->speed_multiplier));
    std::this_thread::sleep_for( dura );
}

/**
 * @brief video_player::is_paused
 * Returns a boolean value representing whether the currently played video is paused.
 * @return if the video is paused or not
 */
bool video_player::is_paused() {
    return video_paused;
}

/**
 * @brief video_player::get_num_frames
 * @return number of frames in video file
 */
int video_player::get_num_frames() {
    return num_frames;
}

/**
 * @brief video_player::set_frame_width
 * @param new_value
 */
void video_player::set_frame_width(int new_value) {
    frame_width = new_value;
}

/**
 * @brief video_player::set_frame_height
 * @param new_value
 */
void video_player::set_frame_height(int new_value) {
    frame_height = new_value;
}

/**
 * @brief video_player::set_playback_frame
 * Moves the playback to the frame specified by frame_num
 * @param frame_num
 */
void video_player::set_playback_frame(int frame_num) {
    if (frame_num < get_num_frames() && frame_num >= 0) {
        current_frame = frame_num;
    }
}

/**
 * @brief video_player::set_playback_speed
 * Sets the speed multiplyer for playback.
 * High value gives slower playback and vice versa.
 * @param speed multiplier value
 */
void video_player::set_speed_multiplier(double mult) {
    this->speed_multiplier = mult;
}

/**
 * @brief video_player::get_speed_multiplier
 * @return speed multiplier
 */
double video_player::get_speed_multiplier() {
    return this->speed_multiplier;
}

/**
 * @brief video_player::dec_playback_speed
 * Decreases the playback speed by a factor of 2
 */
void video_player::dec_playback_speed() {
    if (this->speed_multiplier < MAX_SPEED_MULT) {
        this->set_speed_multiplier(this->get_speed_multiplier()*SPEED_STEP_MULT);
    }
}

/**
 * @brief video_player::inc_playback_speed
 * Increases the playback speed by a factor of 2
 */
void video_player::inc_playback_speed() {
    if (this->speed_multiplier > MIN_SPEED_MULT) {
        this->set_speed_multiplier(this->get_speed_multiplier()/SPEED_STEP_MULT);
    }
}
