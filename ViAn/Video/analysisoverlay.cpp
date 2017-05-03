#include "analysisoverlay.h"

/**
 * @brief AnalysisOverlay::AnalysisOverlay
 */
AnalysisOverlay::AnalysisOverlay() {
}

/**
 * @brief AnalysisOverlay::draw_overlay
 * Draws an overlay with the detected areas, on top of the specified frame.
 * @param frame Frame to draw on.
 * @param frame_nr Number of the frame currently shown in the video.
 * @return Returns the frame with the overlay.
 */
cv::Mat AnalysisOverlay::draw_overlay(cv::Mat &frame, int frame_nr) {
    if (showing_overlay) {
        for (cv::Rect area : detections[frame_nr]) {
            cv::rectangle(frame, area, COLOUR, THICKNESS);
        }
    }
    return frame;
}

/**
 * @brief AnalysisOverlay::add_area
 * @param frame_nr Frame associated with the area.
 * @param rect The detected area.
 */
void AnalysisOverlay::add_area(int frame_nr, cv::Rect rect) {
    detections[frame_nr].push_back(rect);
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
