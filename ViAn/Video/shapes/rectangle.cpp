#include "rectangle.h"

/**
 * @brief Rectangle::Rectangle
 */
Rectangle::Rectangle() : Shapes(SHAPES::RECTANGLE) {
}

/**
 * @brief Rectangle::Rectangle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Rectangle::Rectangle(QColor col, QPoint pos) : Shapes(SHAPES::RECTANGLE, col, pos) {
}

/**
 * @brief Rectangle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Rectangle::draw(cv::Mat &frame) {
    cv::Rect rect(draw_start, draw_end);
    cv::rectangle(frame, rect, color, thickness);
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

QString Rectangle::get_name() {
    return m_name;
}

void Rectangle::set_name(QString name) {
    m_name = name;
}

/**
 * @brief Rectangle::write
 * @param json
 * Writes to a Json object.
 */
void Rectangle::write(QJsonObject& json) {
    write_shape(json);
    json["name"] = m_name;
}

/**
 * @brief Rectangle::read
 * @param json
 * Reads from a Json object.
 */
void Rectangle::read(const QJsonObject& json) {
    read_shape(json);
    m_name = json["name"].toString();
}
