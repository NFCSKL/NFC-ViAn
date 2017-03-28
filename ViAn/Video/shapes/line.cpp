#include "line.h"

/**
 * @brief line::line
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
line::line(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief line::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat line::draw(cv::Mat &frame) {
    cv::line(frame, draw_start, draw_end, colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief line::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void line::handle_new_pos(QPoint pos) {
}
