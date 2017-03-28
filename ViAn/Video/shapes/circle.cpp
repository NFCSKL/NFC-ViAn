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
    int width = std::abs(draw_end.x() - draw_start.x());
    int height = std::abs(draw_end.y() - draw_start.y());
    int x = std::min(draw_start.x(), draw_end.x());
    int y = std::min(draw_start.y(), draw_end.y());
    cv::Point center(x+width/2, y+height/2);
    cv::Size size(width, height);
    cv::RotatedRect roi(center, size, 0);
    cv::ellipse(frame, roi, qcolor2scalar(colour), LINE_THICKNESS);
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
