#include "analysarea.h"

AnalysArea::AnalysArea() {
}

/**
 * @brief AnalysArea::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat AnalysArea::draw(cv::Mat &frame) {
    for (std::pair<cv::Point, cv::Point> line : lines) {
        cv::line(frame, line.first, line.second, colour, LINE_THICKNESS);
    }
    return frame;
}

void AnalysArea::add_point(QPoint pos) {
    points.push_back(Shape::qpoint_to_point(pos));
}

