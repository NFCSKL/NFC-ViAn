#include "Line.h"

/**
 * @brief Line::Line
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Line::Line(QColor col, QPoint pos) : Shape(col, pos) {
}

/**
 * @brief Line::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Line::draw(cv::Mat &frame) {
    cv::line(frame, draw_start, draw_end, colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief Line::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Line::handle_new_pos(QPoint pos) {
}
