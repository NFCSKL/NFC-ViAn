#include "shape.h"
#include <iostream>

/**
 * @brief shape::shape
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
shape::shape(QColor col, QPoint pos) {
    colour = qcolor_to_scalar(col);
    draw_start = qpoint_to_point(pos);
    draw_end = qpoint_to_point(pos);
}

/**
 * @brief shape::update_drawing_pos
 * Updates the position of the end point of the drawing
 * @param pos
 */
void shape::update_drawing_pos(QPoint pos) {
    // Call handle_new_pos first because it might need the old draw_end value.
    handle_new_pos(pos);
    draw_end = qpoint_to_point(pos);
}

/**
 * @brief shape::qcolor_to_scalar
 * Converts QColor to OpenCV Scalar.
 * @param col QColor to be converted.
 * @return Returns converted colour.
 */
cv::Scalar shape::qcolor_to_scalar(QColor col) {
    int r,g,b;
    col.getRgb(&r, &g, &b);
    return cv::Scalar(b,g,r); // swap RGB-->BGR
}

/**
 * @brief shape::qpoint_to_point
 * Converts QPoint to OpenCV Point.
 * @param pnt QPoint to be converted.
 * @return Returns converted Point.
 */
cv::Point shape::qpoint_to_point(QPoint pnt) {
    return cv::Point(pnt.x(), pnt.y());
}
