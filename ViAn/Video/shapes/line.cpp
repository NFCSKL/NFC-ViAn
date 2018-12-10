#include "line.h"

#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

/**
 * @brief Line::Line
 */
Line::Line() : Shapes(SHAPES::LINE) {
    m_name = "Line";
}

/**
 * @brief Line::Line
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Line::Line(QColor col, QPoint pos) : Shapes(SHAPES::LINE, col, pos) {
    m_name = "Line";
}

Line::~Line() {}

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

/**
 * @brief Line::write
 * @param json
 * Writes to a Json object.
 */
void Line::write(QJsonObject& json) {
    write_shape(json);
}

/**
 * @brief Line::read
 * @param json
 * Reads from a Json object.
 */
void Line::read(const QJsonObject& json) {
    read_shape(json);
}
