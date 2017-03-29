#include "Circle.h"

/**
 * @brief Circle::Circle
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Circle::Circle(QColor col, QPoint pos) : Shape(col, pos) {
}

/**
 * @brief Circle::draw
 * Draws the object on top of the specified QImage.
 * @param img QImage to draw on
 */
void Circle::draw(QImage &img) {
    QPainter painter(&img);
    setup_paint_tool(painter);
    int width = draw_end.x() - draw_start.x();
    int height = draw_end.y() - draw_start.y();
    painter.drawEllipse(draw_start.x(), draw_start.y(), width, height);
    painter.end();
}

/**
 * @brief Circle::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Circle::handle_new_pos(QPoint pos) {
}
