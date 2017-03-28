#include "circle.h"

/**
 * @brief circle::circle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
circle::circle(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief circle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat circle::draw(cv::Mat &frame) {
    cv::Rect rect(draw_start, draw_end);
    cv::Size size = rect.size();
    cv::Point center = (rect.br() + rect.tl())*0.5;
    cv::RotatedRect bounding_rect(center, size, 0);
    cv::ellipse(frame, bounding_rect, colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief circle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void circle::handle_new_pos(QPoint pos) {
}
