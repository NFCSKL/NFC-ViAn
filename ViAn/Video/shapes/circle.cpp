#include "circle.h"

/**
 * @brief Circle::Circle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Circle::Circle(QColor col, QPoint pos) : Shape(col, pos) {
}

/**
 * @brief Circle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Circle::draw(cv::Mat &frame) {
    cv::Rect rect(draw_start, draw_end);
    cv::Size size = rect.size();
    cv::Point center = (rect.br() + rect.tl())*0.5;
    cv::RotatedRect bounding_rect(center, size, 0);
    cv::ellipse(frame, bounding_rect, colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief Circle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Circle::handle_new_pos(QPoint pos) {
}
