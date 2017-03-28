#include "arrow.h"

/**
 * @brief arrow::arrow
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
arrow::arrow(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief arrow::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat arrow::draw(cv::Mat &frame) {
    cv::Point p1(draw_start.x(), draw_start.y());
    cv::Point p2(draw_end.x(), draw_end.y());
    cv::arrowedLine(frame, p1, p2, qcolor2scalar(colour), LINE_THICKNESS);
    return frame;
}

/**
 * @brief arrow::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void arrow::handle_new_pos(QPoint pos) {
}
