#include "overlay.h"
#include <iostream>
#include <QInputDialog>

/**
 * @brief Overlay::Overlay
 */
Overlay::Overlay() {
}

/**
 * @brief Overlay::draw_overlay
 * Draws an overlay on top of the specified frame.
 * @param img Frame to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
cv::Mat Overlay::draw_overlay(cv::Mat &frame, int frame_nr) {
    if (show_overlay) {
        foreach (Shape* s, overlays[frame_nr]) {
            frame = s->draw(frame);
        }
    }
    return frame;
}

/**
 * @brief Overlay::toggle_overlay
 * Toggles the showing of the overlay, and if video is paused updates
 * the frame in the GUI to show with/without overlay
 */
void Overlay::toggle_overlay() {
    show_overlay = !show_overlay;
}

/**
 * @brief Overlay::is_showing_overlay
 * @return Returns true if the overlay is currently on
 */
bool Overlay::is_showing_overlay() {
    return show_overlay;
}

/**
 * @brief Overlay::set_showing_overlay
 * @param value
 */
void Overlay::set_showing_overlay(bool value) {
    show_overlay = value;
}

/**
 * @brief Overlay::set_tool
 * Sets the overlay tool's shape.
 * If the tool is the text-tool the user is prompted to wnter a text.
 * @param s
 */
void Overlay::set_tool(SHAPES s) {
    current_shape = s;

    // If the text option is choosen, a string and size will be entered by the user.
    if (s == TEXT) {
        std::string input_string = current_string.toStdString();
        float input_font_scale = current_font_scale;
        CustomDialog dialog("Choose text", NULL);
        dialog.addLabel("Enter values:");
        dialog.addLineEdit ("Text:", &input_string, "Enter a text that can then be used to draw on the overlay.");
        dialog.addDblSpinBoxF("Font scale:", 0.5, 5.0, &input_font_scale, 1, 0.1,
                              "Choose font scale, 0.5 to 5.0 (this value is multiplied with a default font size).");

        // Show the dialog (execution will stop here until the dialog is finished)
        dialog.exec();

        if (!dialog.wasCancelled() && !input_string.empty()) {
            // Not cancelled and not empty field.
            current_string = QString::fromStdString(input_string);
            current_font_scale = input_font_scale;
        }
    }
}

/**
 * @brief Overlay::set_colour
 * Sets the overlay tool's colour.
 * @param col
 */
void Overlay::set_colour(QColor col) {
    current_colour = col;
}

/**
 * @brief Overlay::get_colour
 * @return The currenty choosen colour.
 */
QColor Overlay::get_colour() {
    return current_colour;
}

/**
 * @brief Overlay::get_shape
 * @return The currently choosen shape
 */
SHAPES Overlay::get_shape() {
    return current_shape;
}

/**
 * @brief Overlay::mouse_pressed
 * Creates a drawing shape with the prechoosen colour
 * and shape, if the overlay is visible.
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::mouse_pressed(QPoint pos, int frame_nr) {
    if (show_overlay) {
        switch (current_shape) {
            case RECTANGLE:
                overlays[frame_nr].append(new Rectangle(current_colour, pos));
                break;
            case CIRCLE:
                overlays[frame_nr].append(new Circle(current_colour, pos));
                break;
            case LINE:
                overlays[frame_nr].append(new Line(current_colour, pos));
                break;
            case ARROW:
                overlays[frame_nr].append(new Arrow(current_colour, pos));
                break;
            case PEN:
                overlays[frame_nr].append(new Pen(current_colour, pos));
                break;
            case TEXT:
                overlays[frame_nr].append(new Text(current_colour, pos, current_string, current_font_scale));
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Overlay::mouse_released
 * Ends drawing on the overlay when the mouse is
 * released, if the overlay is visible.
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::mouse_released(QPoint pos, int frame_nr) {
    update_drawing_position(pos, frame_nr);
}

/**
 * @brief Overlay::mouse_moved
 * Updates drawing on the overlay when the mouse is
 * moved, if the overlay is visible.
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::mouse_moved(QPoint pos, int frame_nr) {
    update_drawing_position(pos, frame_nr);
}

/**
 * @brief Overlay::update_drawing_position
 * Updates the position of the end point of the shape currently being drawn
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::update_drawing_position(QPoint pos, int frame_nr) {
    if (show_overlay) {
        // The last appended shape is the one we're currently drawing.
        overlays[frame_nr].last()->update_drawing_pos(pos);
    }
}

/**
 * @brief Overlay::undo
 * Undo the drawings on the overlay, if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::undo(int frame_nr) {
    if (show_overlay && !overlays[frame_nr].isEmpty()) {
        overlays[frame_nr].takeLast(); // Requires a non-empty list.
    }
}

/**
 * @brief Overlay::clear
 * Clear the drawings on the overlay, if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::clear(int frame_nr) {
    if (show_overlay) {
        overlays[frame_nr].clear();
    }
}
