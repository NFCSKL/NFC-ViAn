#include "videoprobe.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <chrono>
#include <sstream>

VideoProbe::VideoProbe(QObject *parent) : QObject(parent) {}



/**
 * @brief VideoProbe::run
 * Seeks through the video at the given path at retrieves the total amount of playable frames
 * @param video_path
 */
void VideoProbe::probe() {
    std::ostringstream out;
    int frames{1};
    cv::VideoCapture capture;
    for (auto& path : m_video_paths) {
        std::cout << path << std::endl;
        capture.open(path, cv::CAP_FFMPEG);

        if (!capture.isOpened()) continue;
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        emit current_video(path);

        while (capture.grab()) {
            emit current_frame(frames);
            frames++;
        }

        int cv_frames = capture.get(cv::CAP_PROP_POS_FRAMES);
        double cv_time = capture.get(cv::CAP_PROP_POS_MSEC);
        auto end = std::chrono::steady_clock::now();
        std::string elapsed = std::to_string(std::chrono::duration<double>(end - start).count());
        std::cout << "TotalFrames::" << frames << " ProbeTime::" << elapsed << " CVFrames::" << cv_frames << " CVTime::" << cv_time << std::endl;
        std::cout << path << " had " << frames << " frames";
        capture.release();
        emit probe_info(path, frames);
        frames = 0;
    }
}

/**
 * @brief VideoProbe::set_paths
 * Set paths to all video to be probed
 * @param video_paths
 */
void VideoProbe::set_paths(const std::vector<std::string> &video_paths) {
    m_video_paths = video_paths;
}
