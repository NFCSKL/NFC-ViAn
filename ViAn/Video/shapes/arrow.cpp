#include "arrow.h"

/**
 * @brief arrow::arrow
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
arrow::arrow(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief arrow::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void arrow::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    // Create the main line of the arrow.
    QLineF line(draw_start.x(), draw_start.y(), draw_end.x(), draw_end.y());
    // Create two lines starting in the main lines end point.
    QLineF line2(draw_end.x(), draw_end.y(), 0, 0); // end point doesn't matter
    QLineF line3(draw_end.x(), draw_end.y(), 0, 0); // end point doesn't matter
    // Set the length of the two shorter lines.
    line2.setLength(10);
    line3.setLength(10);
    // Angle the two shorter lines 45 degrees from the main line.
    line2.setAngle(line.angle()+135);
    line3.setAngle(line.angle()-135);
    // Draw the lines.
    QVector<QLineF> lines{line, line2, line3};
    painter.drawLines(lines);
    painter.end();
}

/**
 * @brief arrow::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return
 */
void arrow::draw(cv::Mat &frame) {
}

/**
 * @brief arrow::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void arrow::handle_new_pos(QPoint pos) {
}
