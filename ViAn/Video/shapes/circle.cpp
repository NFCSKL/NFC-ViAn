#include "circle.h"
#include <QDebug>

Circle::Circle() : Shapes(SHAPES::CIRCLE) {
}

/**
 * @brief Circle::Circle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Circle::Circle(QColor col, QPoint pos) : Shapes(SHAPES::CIRCLE, col, pos) {
}

Circle::~Circle() {}

/**
 * @brief Circle::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Circle::draw(cv::Mat &frame) {
    int diff = draw_end.x - draw_start.x;
    if (diff <= 2 && diff >= -2) {
        draw_end = cv::Point(draw_end.x+(3-diff), draw_end.y);
    }
    cv::Rect rect(draw_start, draw_end);
    cv::Size size = rect.size();
    cv::Point center = (rect.br() + rect.tl())/2;
    cv::RotatedRect bounding_rect(center, size, 0);
    cv::ellipse(frame, bounding_rect, color, thickness);
    return frame;
}

/**
 * @brief Circle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Circle::handle_new_pos(QPoint pos) {
    Q_UNUSED( pos )
}

QString Circle::get_name() {
    return m_name;
}

void Circle::set_name(QString name) {
    m_name = name;
}

/**
 * @brief Circle::write
 * @param json
 * Writes to a Json object.
 */
void Circle::write(QJsonObject& json) {
    write_shape(json);
    json["name"] = m_name;
}

/**
 * @brief Circle::read
 * @param json
 * Reads from a Json object.
 */
void Circle::read(const QJsonObject& json) {
    read_shape(json);
    m_name = json["name"].toString();
}
