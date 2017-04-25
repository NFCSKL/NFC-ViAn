#include "analysisoverlay.h"

/**
 * @brief AnalysisOverlay::AnalysisOverlay
 */
AnalysisOverlay::AnalysisOverlay() {
}

/**
 * @brief AnalysisOverlay::draw_overlay
 * Draws an overlay with the detected areas, on top of the specified frame.
 * @param img Frame to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
cv::Mat AnalysisOverlay::draw_overlay(cv::Mat &frame, int frame_nr) {
    cv::Point draw_start(50, 50);
    cv::Point draw_end(150, 150);
    cv::Rect rect(draw_start, draw_end);
    cv::rectangle(frame, rect, cv::Scalar(255, 0, 0), 5);
    return frame;
}
