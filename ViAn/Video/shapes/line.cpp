#include "line.h"

#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

/**
 * @brief Line::Line
 */
Line::Line() : Shapes(SHAPES::LINE) {
}

/**
 * @brief Line::Line
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Line::Line(QColor col, QPoint pos) : Shapes(SHAPES::LINE, col, pos) {
}

Line::~Line() {}

/**
 * @brief Line::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Line::draw(cv::Mat &frame) {
    cv::line(frame, draw_start, draw_end, color, thickness);
    return frame;
}

/**
 * @brief Line::draw_scaled
 * Scales and draws the object on top the specified frame.
 * @param frame - Frame to draw on.
 * @param anchor - Top left corner in zoomrect, used to scale drawing.
 * @param scale_factor - Zoom factor, used to scale drawing.
 * @return Returns the frame with drawing.
 */
cv::Mat Line::draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) {
    QPoint rot_start = Utility::rotate(Utility::from_cvpoint(draw_start), angle, width, height);
    QPoint rot_end = Utility::rotate(Utility::from_cvpoint(draw_end), angle, width, height);
    cv::line(frame, (Utility::from_qpoint(rot_start)-anchor)*scale_factor, (Utility::from_qpoint(rot_end)-anchor)*scale_factor, color, thickness);
    return frame;
}

/**
 * @brief Line::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Line::handle_new_pos(QPoint pos) {
    Q_UNUSED( pos )
}

QString Line::get_name() {
    return m_name;
}

void Line::set_name(QString name) {
    m_name = name;
}

/**
 * @brief Line::write
 * @param json
 * Writes to a Json object.
 */
void Line::write(QJsonObject& json) {
    write_shape(json);
    json["name"] = m_name;
}

/**
 * @brief Line::read
 * @param json
 * Reads from a Json object.
 */
void Line::read(const QJsonObject& json) {
    read_shape(json);
    m_name = json["name"].toString();
}
