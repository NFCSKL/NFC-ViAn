#include "Shape.h"
#include <iostream>

/**
 * @brief Shape::Shape
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Shape::Shape(QColor col, QPoint pos) {
    colour = col;
    draw_start = pos;
    draw_end = pos;
}

/**
 * @brief Shape::update_drawing_pos
 * Updates the position of the end point of the drawing
 * @param pos
 */
void Shape::update_drawing_pos(QPoint pos) {
    // Call handle_new_pos first because it might need the old draw_end value.
    handle_new_pos(pos);
    draw_end = pos;
}

/**
 * @brief Shape::setup_paint_tool
 * Initiates a painter with the object's colour
 * @param painter
 */
void Shape::setup_paint_tool(QPainter &painter) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(colour);
    painter.setPen(pen);
}
