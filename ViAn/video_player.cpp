#include "video_player.h"
#include <string>
#include <iostream>
#include "mainwindow.h"


using namespace std;
using namespace cv;

static VideoCapture video_player::get_video_from_file(char* file_path) {
    VideoCapture source_video(file_path);
    return source_video;
}

static Mat video_player::play_frame(VideoCapture source_video) {
    Mat frame;
    source_video >> frame;
    return frame;
    double fps = source_video.get(CV_CAP_PROP_FPS);
}

static int video_player::play_video(MainWindow w) {


    const int DEFAULT_FRAME_RATE = 24;
    const std::string window_name = "Video";
    VideoCapture source_video("seq_01.mp4");
    if(!source_video.isOpened()) {
        return -1;
    }

    float frame_rate = source_video.get(CAP_PROP_FPS);  // get frame rate from video
    if (!frame_rate) {
        frame_rate = DEFAULT_FRAME_RATE;
    }

    float delay = 1000 / frame_rate; // delay time between fram updates
    cout << "Frame rate: " << frame_rate << endl;
    cout << "Delay in ms: " << delay << endl;

    source_video.set(CV_CAP_PROP_POS_FRAMES, 700); // move to frame 700 where movement happens


    namedWindow("Video",1); // display window
    for(;;) {
        Mat frame;
        source_video >> frame; // get a new frame from camera

        if (frame.empty()) { // no frame to view
            break;
        }

        imshow(window_name, frame); // draw current frame to the dislay window
        cout << source_video.get(CV_CAP_PROP_POS_FRAMES) << endl;  // current frame number
        int delay = 1000 / 40;
        char c = (char)waitKey(delay); // frame display delay
        if (c == 27) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
