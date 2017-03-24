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
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void pen::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    painter.drawLines(lines);
    painter.end();
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
