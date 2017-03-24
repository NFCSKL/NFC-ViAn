#include "circle.h"

/**
 * @brief circle::circle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
circle::circle(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief circle::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void circle::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    int width = draw_end.x() - draw_start.x();
    int height = draw_end.y() - draw_start.y();
    painter.drawEllipse(draw_start.x(), draw_start.y(), width, height);
    painter.end();
}

/**
 * @brief circle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void circle::handle_new_pos(QPoint pos) {
}
