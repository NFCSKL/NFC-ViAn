#include "pen.h"

/**
 * @brief pen::pen
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
pen::pen(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief shape::update_drawing_pos
 * Updates the position of the end point of the drawing
 * @param pos
 */
void pen::update_drawing_pos(QPoint pos) {
    QLine line(draw_end, pos);
    lines.append(line);
    draw_end = pos;
}

/**
 * @brief pen::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void pen::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    foreach (QLine line, lines) {
        painter.drawLine(line);
    }
    painter.end();
}
