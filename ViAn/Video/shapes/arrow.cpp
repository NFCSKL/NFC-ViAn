#include "arrow.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

/**
 * @brief Arrow::Arrow
 */
Arrow::Arrow() : Shapes(SHAPES::ARROW) {
}

/**
 * @brief Arrow::Arrow
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Arrow::Arrow(QColor col, QPoint pos) : Shapes(SHAPES::ARROW, col, pos) {
}

Arrow::~Arrow() {}

/**
 * @brief Arrow::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Arrow::draw(cv::Mat &frame) {
    cv::arrowedLine(frame, draw_start, draw_end, color, thickness);
    return frame;
}

/**
 * @brief Arrow::draw_scaled
 * Scales and draws the object on top the specified frame.
 * @param frame - Frame to draw on.
 * @param anchor - Top left corner in zoomrect, used to scale drawing.
 * @param scale_factor - Zoom factor, used to scale drawing.
 * @return Returns the frame with drawing.
 */
cv::Mat Arrow::draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor) {
    cv::arrowedLine(frame, (draw_start-anchor)*scale_factor, (draw_end-anchor)*scale_factor, color, thickness);
    return frame;
}

/**
 * @brief Arrow::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Arrow::handle_new_pos(QPoint pos) {
    Q_UNUSED( pos )
}

QString Arrow::get_name() {
    return m_name;
}

void Arrow::set_name(QString name) {
    m_name = name;
}

/**
 * @brief Arrow::write
 * @param json
 * Writes to a Json object.
 */
void Arrow::write(QJsonObject& json) {
    write_shape(json);
    json["name"] = m_name;
}

/**
 * @brief Arrow::read
 * @param json
 * Reads from a Json object.
 */
void Arrow::read(const QJsonObject& json) {
    read_shape(json);
    m_name = json["name"].toString();
}
