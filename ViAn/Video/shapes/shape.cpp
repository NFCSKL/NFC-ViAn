#include "shape.h"
#include <iostream>

/**
 * @brief shape::shape
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
shape::shape(QColor col, QPoint pos) {
    colour = col;
    draw_start = pos;
    draw_end = pos;
}

/**
 * @brief shape::update_drawing_pos
 * Updates the position of the end point of the drawing
 * @param pos
 */
void shape::update_drawing_pos(QPoint pos) {
    // Call handle_new_pos first because it might need the old draw_end value.
    handle_new_pos(pos);
    draw_end = pos;
}

/**
 * @brief shape::setup_paint_tool
 * Initiates a painter with the object's colour
 * @param painter
 */
void shape::setup_paint_tool(QPainter &painter) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(colour);
    painter.setPen(pen);
}

cv::Scalar shape::qcolor2scalar(QColor col) {
    int r,g,b;
    col.getRgb(&r, &g, &b);
    return cv::Scalar(b,g,r); // swap RGB-->BGR
}
