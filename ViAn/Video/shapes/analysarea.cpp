#include "analysarea.h"

#include "shapes.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

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

        // Create array of points for the polygon.
        for (int i = 0; i < size; i++) {
            rook_points[0][i] = points->at(i);
        }
        const cv::Point* ppt[1] = {rook_points[0]};
        int npt[] = {size};

        if (include_area_to_analyse) {
            // Frame with white polygon, otherwise zeros.
            cv::Mat inside_area;
            inside_area = frame.zeros(frame.rows, frame.cols, frame.type());
            cv::fillPoly(inside_area, ppt, npt, 1, cv::Scalar(255, 255, 255));

            // Inverted frame.
            cv::Mat outside_area;
            cv::bitwise_not(inside_area, outside_area);

            // Completely white frame.
            cv::Mat white_frame = frame.zeros(frame.rows, frame.cols, frame.type());
            white_frame = cv::Scalar(255,255,255);

            // Original frame with opacity.
            cv::Mat opacity_frame;
            cv::addWeighted(white_frame, Shapes::ALPHA, frame, 1.0 - Shapes::ALPHA, 0.0, opacity_frame);

            cv::Mat original_cut, opacity_cut;

            // Cut out the polygon from the frame with opacity.
            cv::bitwise_and(outside_area, opacity_frame, opacity_cut);

            // Cut out everything but the polygon from the original frame.
            cv::bitwise_and(inside_area, frame, original_cut);

            // Unite the images using OR.
            cv::bitwise_or(opacity_cut, original_cut, frame);
        } else {
            // Copy the frame.
            cv::Mat area;
            frame.copyTo(area);

            // Draw polygon on the copied frame.
            cv::fillPoly(area, ppt, npt, 1, cv::Scalar(255, 255, 255));

            // Add opacity and blend with the original frame.
            cv::addWeighted(area, Shapes::ALPHA, frame, 1.0 - Shapes::ALPHA, 0.0, frame);
        }
        // Draw contour of the polygon.
        cv::polylines(frame, ppt, npt, 1, true, cv::Scalar(255, 0, 0), Shapes::LINE_THICKNESS);

        // Draw a circle indicating the last choosen point.
        int RADIUS = 8;
        cv::circle(frame, points->back(), RADIUS, cv::Scalar(0, 255, 255), Shapes::LINE_THICKNESS);
    }
    return frame;
}

/**
 * @brief AnalysArea::add_point
 * Adds point to the area that should be selected.
 * @param pos The new position to be added.
 */
void AnalysArea::add_point(QPoint pos) {
    points->push_back(Shapes::qpoint_to_point(pos));
}

/**
 * @brief AnalysArea::invert_area
 * Inverts the area being drawn.
 */
void AnalysArea::invert_area() {
    include_area_to_analyse = !include_area_to_analyse;
}

/**
 * @brief AnalysArea::including_area
 * @return Returns true if the area should be included in the
 *         analysis, false if it should be excluded.
 */
bool AnalysArea::is_including_area() {
    return include_area_to_analyse;
}

/**
 * @brief AnalysArea::get_polygon
 * @return Returns pointer to the choosen polygon.
 */
std::vector<cv::Point>* AnalysArea::get_polygon() {
    return points;
}

/**
 * @brief AnalysArea::undo
 * Removes the last added point from the selected area.
 */
void AnalysArea::undo() {
    if (!points->empty()) {
        points->pop_back();
    }
}

/**
 * @brief AnalysArea::clear
 * Removes all added points from the selected area.
 */
void AnalysArea::clear() {
    points->clear();
}
