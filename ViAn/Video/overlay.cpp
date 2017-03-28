#include "overlay.h"
#include <iostream>
#include <QInputDialog>

/**
 * @brief overlay::overlay
 */
overlay::overlay() {
}

/**
 * @brief overlay::draw_overlay
 * Draws an overlay on top of the specified frame.
 * @param img Frame to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
cv::Mat overlay::draw_overlay(cv::Mat &frame, int frame_nr) {
    if (show_overlay) {
        foreach (shape* s, overlays[frame_nr]) {
            frame = s->draw(frame);
        }
    }
    return frame;
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
 * If the tool is the text-tool the user is prompted to wnter a text.
 * @param s
 */
void overlay::set_tool(SHAPES s) {
    if (s == TEXT) {
        bool ok;
        current_string = QInputDialog::getText(NULL, "Text chooser", "Enter a text:", QLineEdit::Normal, QString(), &ok);
        if (!ok || current_string.isEmpty()) {
            // Cancelled or empty field.
            return;
        }
    }
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
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::mouse_pressed(QPoint pos, int frame_nr) {
    if (show_overlay) {
        switch (current_shape) {
            case RECTANGLE:
                overlays[frame_nr].append(new rectangle(current_colour, pos));
                break;
            case CIRCLE:
                overlays[frame_nr].append(new circle(current_colour, pos));
                break;
            case LINE:
                overlays[frame_nr].append(new line(current_colour, pos));
                break;
            case ARROW:
                overlays[frame_nr].append(new arrow(current_colour, pos));
                break;
            case PEN:
                overlays[frame_nr].append(new pen(current_colour, pos));
                break;
            case TEXT:
                overlays[frame_nr].append(new text(current_colour, pos, current_string));
                break;
            default:
                break;
        }
    }
}

/**
 * @brief overlay::mouse_released
 * Ends drawing on the overlay when the mouse is
 * released, if the overlay is visible.
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::mouse_released(QPoint pos, int frame_nr) {
    update_drawing_position(pos, frame_nr);
}

/**
 * @brief overlay::mouse_moved
 * Updates drawing on the overlay when the mouse is
 * moved, if the overlay is visible.
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::mouse_moved(QPoint pos, int frame_nr) {
    update_drawing_position(pos, frame_nr);
}

/**
 * @brief overlay::update_drawing_position
 * Updates the position of the end point of the shape currently being drawn
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::update_drawing_position(QPoint pos, int frame_nr) {
    if (show_overlay) {
        // The last appended shape is the one we're currently drawing.
        overlays[frame_nr].last()->update_drawing_pos(pos);
    }
}

/**
 * @brief overlay::undo
 * Undo the drawings on the overlay, if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::undo(int frame_nr) {
    if (show_overlay && !overlays[frame_nr].isEmpty()) {
        overlays[frame_nr].takeLast(); // Requires a non-empty list.
    }
}

/**
 * @brief overlay::clear
 * Clear the drawings on the overlay, if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::clear(int frame_nr) {
    if (show_overlay) {
        overlays[frame_nr].clear();
    }
}
