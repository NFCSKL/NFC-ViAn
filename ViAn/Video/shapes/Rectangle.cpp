#include "Rectangle.h"

/**
 * @brief Rectangle::Rectangle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Rectangle::Rectangle(QColor col, QPoint pos) : Shape(col, pos) {
}

/**
 * @brief Rectangle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Rectangle::draw(cv::Mat &frame) {
    cv::Rect rect(draw_start, draw_end);
    cv::rectangle(frame, rect, colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief Rectangle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Rectangle::handle_new_pos(QPoint pos) {
}
