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
        switch (shape) {
            case RECTANGLE:
                painter.drawRect(400,200,400,400);
                break;
            case CIRCLE:
                painter.drawEllipse(400,200,400,400);
                break;
            case LINE:
                painter.drawLine(400,200,400,400);
                break;
            case ARROW:
                {
                    QLineF line(400,200,800,600);
                    line.normalVector();
                    QLineF line2(800,600,400,200);
                    QLineF line3(800,600,400,200);
                    line2.setLength(10);
                    line3.setLength(10);
                    line2.setAngle(line.angle()+145);
                    line3.setAngle(line.angle()-145);
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
