#include "videoprobe.h"

VideoProbe::VideoProbe() {}

/**
 * @brief VideoProbe::run
 * Seeks through the video at the given path at retrieves the total amount of playable frames
 * @param video_path
 */
VideoProbe::run(const std::string &video_path){
    int frames{};
    cv::VideoCapture capture(video_path, cv::CAP_FFMPEG);

    if (!capture.isOpened()) return frames;
    while (capture.grab()) {frames++;}
    return frames;
}
