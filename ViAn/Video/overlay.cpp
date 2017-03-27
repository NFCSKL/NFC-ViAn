#include "overlay.h"
#include <iostream>
#include <QInputDialog>

/**
 * @brief overlay::overlay
 */
overlay::overlay() {
}

/**
 * @brief overlay::load_video
 * Handles updating of the overlay, when a video has been loaded.
 * @param width Width of the video.
 * @param height Height of the video.
 */
void overlay::load_video(int width, int height) {
    zoom_area->reset_zoom_area(width, height);
}

/**
 * @brief overlay::draw_overlay
 * Draws an overlay on top of the specified frame,
 * and zooms in the frame.
 * @param frame Frame to draw on.
 * @return Returns the frame including the zoom and overlay.
 */
cv::Mat overlay::draw_overlay(cv::Mat &frame) {
    //cv::Rect roi(400, 100, 600, 300);
    cv::Rect roi = zoom_area->get_zoom_area();

    cv::Mat zoomed_frame;
    resize(frame(roi), zoomed_frame, frame.size());
    return zoomed_frame;
}

/**
 * @brief overlay::draw_overlay
 * Draws an overlay on top of the specified QImage.
 * @param img QImage to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
void overlay::draw_overlay(QImage &img, int frame_nr) {
    if (show_overlay) {
        foreach (shape* s, overlays[frame_nr]) {
            s->draw(img);
        }
    }
    if (choosing_zoom_area) {
        zoom_area->draw(img);
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
 * If the tool is the text-tool the user is prompted to wnter a text.
 * @param s
 */
void overlay::set_tool(SHAPES s) {
    current_shape = s;
    if (s == TEXT) {
        current_string = QInputDialog::getText(NULL, "Text chooser", "Enter a text:");
    }
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
    if (choosing_zoom_area) {
        zoom_area->set_start_pos(pos);
        zoom_area->update_drawing_pos(pos);
        return;
    }
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
    update_drawing_position(pos, frame_nr); // Needs to be done before resetting choosing_zoom_area.
    if (choosing_zoom_area) {
        zoom_area->area_choosen();
        choosing_zoom_area = false; // Reset the mode. You can only choose a zoom area during one drag with the mouse.
    }
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
    if (choosing_zoom_area) {
        zoom_area->update_drawing_pos(pos);
        return; // While choosing zoom area the regular drawings should not be affected.
    }
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
    if (show_overlay) {
        if (overlays[frame_nr].isEmpty()) {
            return;
        }
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

/**
 * @brief overlay::zoom_in
 * Sets a state in the video overlay
 * for the user to choose an area.
 */
void overlay::zoom_in() {

    // TODO: Should not be like this.
    //zoom_out();

    choosing_zoom_area = true;
}

/**
 * @brief overlay::zoom_out
 * Resets zoom level to the full video size.
 */
void overlay::zoom_out() {
    zoom_area->reset_zoom_area();
}
