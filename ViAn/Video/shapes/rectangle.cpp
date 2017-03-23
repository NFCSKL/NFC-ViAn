#include "rectangle.h"

/**
 * @brief rectangle::rectangle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
rectangle::rectangle(QColor col, QPoint pos) : shape(col, pos) {
}

/**
 * @brief rectangle::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void rectangle::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    int width = draw_end.x() - draw_start.x();
    int height = draw_end.y() - draw_start.y();
    painter.drawRect(draw_start.x(), draw_start.y(), width, height);
    painter.end();
}
