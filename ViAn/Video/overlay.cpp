#include "overlay.h"

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
 * If the tool is the text-tool the user is prompted
 * to enter a text and a font scale.
 * @param s New tool to be set.
 */
void Overlay::set_tool(SHAPES s) {
    current_shape = s;

    // If the text option is chosen, a string and size will be entered by the user.
    if (s == TEXT) {
        std::string input_string = current_string.toStdString();
        float input_font_scale = current_font_scale;
        CustomDialog dialog("Choose text", NULL);
        dialog.addLabel("Enter values:");
        dialog.addLineEdit ("Text:", &input_string, "Enter a text that can then be used to draw on the overlay.");
        dialog.addDblSpinBoxF("Font scale:", Text::FONT_SCALE_MIN, Text::FONT_SCALE_MAX,
                              &input_font_scale, Text::FONT_SCALE_DECIMALS, Text::FONT_SCALE_STEP,
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
 * @param col New colour to be set.
 */
void Overlay::set_colour(QColor col) {
    current_colour = col;
}

/**
 * @brief Overlay::get_colour
 * @return The currenty chosen colour.
 */
QColor Overlay::get_colour() {
    return current_colour;
}

/**
 * @brief Overlay::get_tool
 * @return The currently chosen tool
 */
SHAPES Overlay::get_tool() {
    return current_shape;
}

/**
 * @brief Overlay::get_empty_shape
 * @param shape_type
 * @return Returns a new shape of the given type.
 */
Shape* Overlay::get_empty_shape(SHAPES shape_type) {
    switch (shape_type) {
        case RECTANGLE:
            return new Rectangle();
            break;
        case CIRCLE:
            return new Circle();
            break;
        case LINE:
            return new Line();
            break;
        case ARROW:
            return new Arrow();
            break;
        case PEN:
            return new Pen();
            break;
        case TEXT:
            return new Text();
            break;
        default:
            return nullptr;
            break;
    }
}

/**
 * @brief Overlay::mouse_pressed
 * Creates a drawing shape with the prechosen colour
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

/**
 * @brief Overlay::read
 * @param json
 * Reads the overlay from a Json object.
 */
void Overlay::read(const QJsonObject& json) {
    QJsonArray json_overlays = json["overlays"].toArray();
    for(int i = 0; i != json_overlays.size(); i++) {
        // For each overlay, get the associated frame number
        // and all drawings.
        QJsonObject json_overlay = json_overlays[i].toObject();
        int frame_nr = json_overlay["frame"].toInt();
        QJsonArray json_drawings = json_overlay["drawings"].toArray();
        for(int i = 0; i != json_drawings.size(); i++) {
            // For each drawing, get the shape type and read
            // a shape of that type.
            QJsonObject json_shape = json_drawings[i].toObject();
            int shape_i = json_shape["shape"].toInt();
            SHAPES shape_t = static_cast<SHAPES>(shape_i);
            Shape* shape = get_empty_shape(shape_t);
            shape->read(json_shape);
            overlays[frame_nr].append(shape);
        }
    }
}

/**
 * @brief Overlay::write
 * @param json
 * Writes the overlay to a Json object.
 */
void Overlay::write(QJsonObject& json) {
    QJsonArray json_overlays;
    for (auto const& map_entry : overlays) {
        QJsonObject json_overlay;
        QJsonArray json_drawings;
        foreach (Shape* s, map_entry.second) { // Second member is the value, i.e. the drawings.
            QJsonObject json_shape;
            s->write(json_shape);
            json_drawings.append(json_shape);
        }
        json_overlay["frame"] = map_entry.first; // First member is the key, i.e. the frame number.
        json_overlay["drawings"] = json_drawings;
        json_overlays.append(json_overlay);
    }
    json["overlays"] = json_overlays;
}

