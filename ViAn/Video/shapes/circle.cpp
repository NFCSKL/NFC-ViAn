#include "circle.h"

#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

Circle::Circle() : Shapes(SHAPES::CIRCLE) {
    m_name = "Circle";
}

/**
 * @brief Circle::Circle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Circle::Circle(QColor col, QPoint pos) : Shapes(SHAPES::CIRCLE, col, pos) {
    m_name = "Circle";
}

Circle::~Circle() {}

/**
 * @brief Circle::draw_scaled
 * Scales and draws the object on top the specified frame.
 * @param frame - Frame to draw on.
 * @param anchor - Top left corner in zoomrect, used to scale drawing.
 * @param scale_factor - Zoom factor, used to scale drawing.
 * @return Returns the frame with drawing.
 */
cv::Mat Circle::draw_scaled(cv::Mat &frame, cv::Point anchor,
                            double scale_factor, int angle,
                            bool flip_h, bool flip_v,
                            int width, int height) {
    QPoint rot_start = Utility::from_cvpoint(draw_start);
    QPoint rot_end = Utility::from_cvpoint(draw_end);
    rot_start = Utility::flip(rot_start, flip_h, flip_v, width, height);
    rot_end = Utility::flip(rot_end, flip_h, flip_v, width, height);
    rot_start = Utility::rotate(rot_start, angle, width, height);
    rot_end = Utility::rotate(rot_end, angle, width, height);
    cv::Rect rect((Utility::from_qpoint(rot_start)-anchor)*scale_factor, (Utility::from_qpoint(rot_end)-anchor)*scale_factor);
    cv::Size size = rect.size();
    cv::Point center = (rect.br() + rect.tl())*0.5;
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

/**
 * @brief Circle::write
 * @param json
 * Writes to a Json object.
 */
void Circle::write(QJsonObject& json) {
    write_shape(json);
}

/**
 * @brief Circle::read
 * @param json
 * Reads from a Json object.
 */
void Circle::read(const QJsonObject& json) {
    read_shape(json);
}
