#include "arrow.h"

#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

/**
 * @brief Arrow::Arrow
 */
Arrow::Arrow() : Shapes(SHAPES::ARROW) {
    m_name = "Arrow";
}

/**
 * @brief Arrow::Arrow
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Arrow::Arrow(QColor col, QPoint pos) : Shapes(SHAPES::ARROW, col, pos) {
    m_name = "Arrow";
}

Arrow::~Arrow() {}

/**
 * @brief Arrow::draw_scaled
 * Scales and draws the object on top the specified frame.
 * @param frame - Frame to draw on.
 * @param anchor - Top left corner in zoomrect, used to scale drawing.
 * @param scale_factor - Zoom factor, used to scale drawing.
 * @return Returns the frame with drawing.
 */
cv::Mat Arrow::draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) {
    QPoint rot_start = Utility::rotate(Utility::from_cvpoint(draw_start), angle, width, height);
    QPoint rot_end = Utility::rotate(Utility::from_cvpoint(draw_end), angle, width, height);
    cv::arrowedLine(frame, (Utility::from_qpoint(rot_start)-anchor)*scale_factor, (Utility::from_qpoint(rot_end)-anchor)*scale_factor, color, thickness);
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

/**
 * @brief Arrow::write
 * @param json
 * Writes to a Json object.
 */
void Arrow::write(QJsonObject& json) {
    write_shape(json);
}

/**
 * @brief Arrow::read
 * @param json
 * Reads from a Json object.
 */
void Arrow::read(const QJsonObject& json) {
    read_shape(json);
}
