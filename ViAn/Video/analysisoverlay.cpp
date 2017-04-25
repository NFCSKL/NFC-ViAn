#include "analysisoverlay.h"

/**
 * @brief AnalysisOverlay::AnalysisOverlay
 */
AnalysisOverlay::AnalysisOverlay() {
    // Temporary areas to show, since there's no analysis yet.
    add_area(2, cv::Point(50,50), cv::Point(150,150));
    add_area(3, cv::Point(50,50), cv::Point(150,150));
    add_area(4, cv::Point(50,50), cv::Point(150,150));
    add_area(5, cv::Point(50,50), cv::Point(150,150));
    add_area(6, cv::Point(50,50), cv::Point(150,150));
    add_area(7, cv::Point(50,50), cv::Point(150,150));
    add_area(8, cv::Point(50,50), cv::Point(150,150));
    add_area(9, cv::Point(50,50), cv::Point(150,150));
    add_area(10, cv::Point(150,150), cv::Point(250,170));
}

/**
 * @brief AnalysisOverlay::draw_overlay
 * Draws an overlay with the detected areas, on top of the specified frame.
 * @param img Frame to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
cv::Mat AnalysisOverlay::draw_overlay(cv::Mat &frame, int frame_nr) {
    if (showing_overlay) {
        for (std::pair<cv::Point, cv::Point> area : detections[frame_nr]) {
            cv::Rect rect(area.first, area.second);
            cv::rectangle(frame, rect, cv::Scalar(255, 0, 0), 5);
        }
    }
    return frame;
}

/**
 * @brief AnalysisOverlay::add_area
 * @param frame_nr Frame associated with the area.
 * @param start Start point.
 * @param end End point.
 */
void AnalysisOverlay::add_area(int frame_nr, cv::Point start, cv::Point end) {
    std::pair <cv::Point, cv::Point> pair(start, end);
    detections[frame_nr].push_back(pair);
}

/**
 * @brief AnalysisOverlay::is_showing_overlay
 * @return Returns true if the analysis overlay is showing, else false.
 */
bool AnalysisOverlay::is_showing_overlay() {
    return showing_overlay;
}

/**
 * @brief AnalysisOverlay::toggle_showing
 */
void AnalysisOverlay::toggle_showing() {
    showing_overlay = !showing_overlay;
}
