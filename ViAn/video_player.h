#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;


class video_player
{
public:
    video_player();
    static get_video_from_file(char* file_path);
    static play_frame(VideoCapture source_video);
};

#endif // VIDEO_PLAYER_H
