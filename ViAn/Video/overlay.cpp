#include "overlay.h"
#include <iostream>

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
        foreach (shape* s, drawings) {
            s->draw(img);
        }
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
 * @brief overlay::set_tool
 * Sets the overlay tool's shape.
 * @param s
 */
void overlay::set_tool(SHAPES s) {
    current_shape = s;
}

/**
 * @brief overlay::set_colour
 * Sets the overlay tool's colour.
 * @param col
 */
void overlay::set_colour(QColor col) {
    current_colour = col;
}

/**
 * @brief overlay::get_colour
 * @return The currenty choosen colour.
 */
QColor overlay::get_colour() {
    return current_colour;
}

/**
 * @brief overlay::get_shape
 * @return The currently choosen shape
 */
SHAPES overlay::get_shape() {
    return current_shape;
}

/**
 * @brief overlay::mouse_pressed
 * Creates a drawing shape with the prechoosen colour
 * and shape, if the overlay is visible.
 * @param pos coordinates
 */
void overlay::mouse_pressed(QPoint pos) {
    if (show_overlay) {
        switch (current_shape) {
            case RECTANGLE:
                drawings.append(new rectangle(current_colour, pos));
                break;
            case CIRCLE:
                drawings.append(new circle(current_colour, pos));
                break;
            case LINE:
                drawings.append(new line(current_colour, pos));
                break;
            case ARROW:
                drawings.append(new arrow(current_colour, pos));
                break;
            case PEN:
                drawings.append(new pen(current_colour, pos));
                break;
            default:
                break;
        }
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

/**
 * @brief overlay::update_drawing_position
 * Updates the position of the end point of the shape currently being drawn
 * @param pos
 */
void overlay::update_drawing_position(QPoint pos) {
    if (show_overlay) {
        // The last appended shape is the one we're currently drawing.
        drawings.last()->update_drawing_pos(pos);
    }
}

/**
 * @brief overlay::undo
 * Undo the drawings on the overlay.
 */
void overlay::undo() {
    drawings.takeLast();
}

/**
 * @brief overlay::clear
 * Clear the drawings on the overlay.
 */
void overlay::clear() {
    drawings.clear();
}
