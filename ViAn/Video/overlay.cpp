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
        QPainter painter(&img);
        QPen pen;
        pen.setWidth(3);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawRect(400,200,400,400);
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
