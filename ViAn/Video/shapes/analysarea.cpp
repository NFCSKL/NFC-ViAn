#include "analysarea.h"

/**
 * @brief AnalysArea::AnalysArea
 */
AnalysArea::AnalysArea() {
}

/**
 * @brief AnalysArea::~AnalysArea
 */
AnalysArea::~AnalysArea() {
    delete points;
}

/**
 * @brief AnalysArea::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat AnalysArea::draw(cv::Mat &frame) {
    int size = points->size();
    // The drawing requires at least one point to draw.
    if (size > 0) {
        cv::Point rook_points[1][size];

        // Copy the frame.
        cv::Mat area;
        frame.copyTo(area);

        // Create array of points for the polygon.
        for (int i = 0; i < size; i++) {
            rook_points[0][i] = points->at(i);
        }
        const cv::Point* ppt[1] = {rook_points[0]};
        int npt[] = {size};

        // Draw polygon on the copied frame.
        cv::fillPoly(area, ppt, npt, 1, cv::Scalar(255, 255, 255));
        cv::polylines(area, ppt, npt, 1, true, cv::Scalar(255, 0, 0), Shape::LINE_THICKNESS);

        // Add opacity and blend with the original frame.
        double alpha = 0.6;
        cv::addWeighted(area, alpha, frame, 1.0 - alpha , 0.0, frame);
    }
    return frame;
}

/**
 * @brief AnalysArea::add_point
 * Adds point to the area that should be selected.
 * @param pos The new position to be added.
 */
void AnalysArea::add_point(QPoint pos) {
    points->push_back(Shape::qpoint_to_point(pos));
}

/**
 * @brief AnalysArea::undo
 * Removes the last added point from the selected area.
 */
void AnalysArea::undo() {
    points->pop_back();
}

/**
 * @brief AnalysArea::clear
 * Removes all added points from the selected area.
 */
void AnalysArea::clear() {
    points->clear();
}
