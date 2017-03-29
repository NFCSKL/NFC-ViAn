#include "Pen.h"

/**
 * @brief Pen::Pen
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Pen::Pen(QColor col, QPoint pos) : Shape(col, pos) {
}

/**
 * @brief Pen::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void Pen::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    painter.drawLines(lines);
    painter.end();
}

/**
 * @brief Pen::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Pen::handle_new_pos(QPoint pos) {
    QLine line(draw_end, pos);
    lines.append(line);
}
