#include "Line.h"

/**
 * @brief Line::Line
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Line::Line(QColor col, QPoint pos) : Shape(col, pos) {
}

/**
 * @brief Line::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void Line::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    painter.drawLine(draw_start.x(), draw_start.y(), draw_end.x(), draw_end.y());
    painter.end();
}

/**
 * @brief Line::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Line::handle_new_pos(QPoint pos) {
}
