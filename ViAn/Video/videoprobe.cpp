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
    int frames{0};
    cv::VideoCapture capture;
    for (auto& path : m_video_paths) {
        std::cout << path << std::endl;
        capture.open(path);


        if (!capture.isOpened()) continue;
        emit current_video(path);

        while (capture.grab()) {
            emit current_frame(frames);
            frames++;
        }

        // Assumes that the ratio between fps and frame count is correct
        int cv_frames = capture.get(cv::CAP_PROP_FRAME_COUNT);
        int cv_fps = capture.get(cv::CAP_PROP_FPS);
        int cv_time = capture.get(cv::CAP_PROP_POS_MSEC);

        double calc_time = ((double)cv_frames / capture.get(cv::CAP_PROP_FPS)) * 1000; // ms

        std::cout << "OpenCV frame count : " << cv_frames << std::endl;
        std::cout << "OpenCV FPS: " << cv_fps << std::endl;
        std::cout << "OpenCV duration: " << cv_time << std::endl;
        std::cout << "Calculated frame count: " << frames << std::endl;
        std::cout << "Calculated FPS: " << frames / (calc_time / 1000) << std::endl;
        std::cout << "Calculated duration: " << calc_time << std::endl;


        capture.release();
        emit probe_info(path, frames, (int)calc_time);
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
