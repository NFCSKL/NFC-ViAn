#include "pen.h"

/**
 * @brief pen::pen
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
pen::pen(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief pen::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat pen::draw(cv::Mat &frame) {
    for (QLine line : lines) {
        cv::Point p1(line.x1(), line.y1());
        cv::Point p2(line.x2(), line.y2());
        cv::line(frame, p1, p2, qcolor2scalar(colour), LINE_THICKNESS);
    }
    return frame;
}

/**
 * @brief pen::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void pen::handle_new_pos(QPoint pos) {
    QLine line(draw_end, pos);
    lines.append(line);
}
