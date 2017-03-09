#include "overlay.h"
#include <iostream>
#include <qpainter.h>

/**
 * @brief overlay::overlay
 */
overlay::overlay() {
}

/**
 * @brief overlay::draw_overlay
 * Draws an overlay on top of the specified QImage.
 * @param img QImage to draw on
 */
void overlay::draw_overlay(QImage &img) {
    if (show_overlay) {
        // For now this function just paints static shapes
        QPainter painter(&img);
        QPen pen;
        pen.setWidth(3);
        pen.setColor(colour);
        painter.setPen(pen);
        int width = draw_end.x() - draw_start.x();
        int height = draw_end.y() - draw_start.y();
        switch (shape) {
            case RECTANGLE:
                painter.drawRect(draw_start.x(), draw_start.y(), width, height);
                break;
            case CIRCLE:
                painter.drawEllipse(draw_start.x(), draw_start.y(), width, height);
                break;
            case LINE:
                painter.drawLine(draw_start.x(), draw_start.y(), width, height);
                break;
            case ARROW:
                {
                    // Create the main line of the arrow.
                    QLineF line(draw_start.x(), draw_start.y(), draw_end.x(), draw_end.y());
                    // Create two lines starting in the main lines end point.
                    QLineF line2(draw_end.x(), draw_end.y(), draw_start.x(), draw_start.y());
                    QLineF line3(draw_end.x(), draw_end.y(), draw_start.x(), draw_start.y());
                    // Set the length of the two shorter lines.
                    line2.setLength(10);
                    line3.setLength(10);
                    // Angle the two shorter lines 45 degrees from the main line.
                    line2.setAngle(line.angle()+135);
                    line3.setAngle(line.angle()-135);
                    // Draw the lines.
                    QVector<QLineF> lines{line, line2, line3};
                    painter.drawLines(lines);
                    break;
                }
            default:
                break;
        }
        painter.end();
    }
}

/**
 * @brief overlay::toggle_overlay
 * Toggles the showing of the overlay, and if video is paused updates
 * the frame in the GUI to show with/without overlay
 */
void overlay::toggle_overlay() {
    show_overlay = !show_overlay;
}

/**
 * @brief overlay::is_showing_overlay
 * @return Returns true if the overlay is currently on
 */
bool overlay::is_showing_overlay() {
    return show_overlay;
}

/**
 * @brief overlay::set_showing_overlay
 * @param value
 */
void overlay::set_showing_overlay(bool value) {
    show_overlay = value;
}

/**
 * @brief overlay::set_overlay_tool
 * Sets the overlay tool's shape.
 * @param s
 */
void overlay::set_overlay_tool(SHAPES s) {
    shape = s;
}

/**
 * @brief overlay::set_overlay_colour
 * Sets the overlay tool's colour.
 * @param col
 */
void overlay::set_overlay_colour(QColor col) {
    colour = col;
}

/**
 * @brief overlay::mouse_pressed
 * Starts drawing on the overlay when the mouse is
 * pressed, if the overlay is visible.
 * @param pos coordinates
 */
void overlay::mouse_pressed(QPoint pos) {
    if (show_overlay) {
        draw_start = pos;
        draw_end = pos;
    }
}

/**
 * @brief overlay::mouse_pressed
 * Ends drawing on the overlay when the mouse is
 * released, if the overlay is visible.
 * @param pos coordinates
 */
void overlay::mouse_released(QPoint pos) {
    update_drawing_position(pos);
}

/**
 * @brief overlay::mouse_moved
 * Updates drawing on the overlay when the mouse is
 * moved, if the overlay is visible.
 * @param pos coordinates
 */
void overlay::mouse_moved(QPoint pos) {
    update_drawing_position(pos);
}

void overlay::update_drawing_position(QPoint pos) {
    if (show_overlay) {
        draw_end = pos;
    }
}
