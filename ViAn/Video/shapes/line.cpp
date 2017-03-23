#include "line.h"

/**
 * @brief line::line
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
line::line(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief line::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void line::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    painter.drawLine(draw_start.x(), draw_start.y(), draw_end.x(), draw_end.y());
    painter.end();
}
