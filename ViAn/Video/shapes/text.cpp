#include "text.h"

/**
 * @brief text::text
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
text::text(QColor col, QPoint pos, QString strng) : shape(col, pos) {
    string = strng;
}

/**
 * @brief text::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat text::draw(cv::Mat &frame) {
    cv::putText(frame, string.toStdString(), draw_end, cv::FONT_HERSHEY_SIMPLEX, 1,
                colour, LINE_THICKNESS);
    return frame;
}

/**
 * @brief text::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void text::handle_new_pos(QPoint pos) {
}
