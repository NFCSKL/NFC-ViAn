#include "rectangle.h"

/**
 * @brief Rectangle::Rectangle
 */
Rectangle::Rectangle() : Shape(SHAPES::RECTANGLE) {
}

/**
 * @brief Rectangle::Rectangle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Rectangle::Rectangle(QColor col, QPoint pos) : Shape(SHAPES::RECTANGLE, col, pos) {
}

/**
 * @brief Rectangle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Rectangle::draw(cv::Mat &frame) {
    cv::Rect rect(draw_start, draw_end);
    cv::rectangle(frame, rect, color, LINE_THICKNESS);
    return frame;
}

/**
 * @brief Rectangle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Rectangle::handle_new_pos(QPoint pos) {
    Q_UNUSED( pos )
}

/**
 * @brief Rectangle::write
 * @param json
 * Writes to a Json object.
 */
void Rectangle::write(QJsonObject& json) {
    write_shape(json);
}

/**
 * @brief Rectangle::read
 * @param json
 * Reads from a Json object.
 */
void Rectangle::read(const QJsonObject& json) {
    read_shape(json);
}
