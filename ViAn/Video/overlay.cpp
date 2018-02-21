#include "overlay.h"
#include <QDebug>

/**
 * @brief Overlay::Overlay
 */
Overlay::Overlay() {}

/**
 * @brief Overlay::draw_overlay
 * Draws an overlay on top of the specified frame.
 * @param img Frame to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::draw_overlay(cv::Mat &frame, int frame_nr) {
    if (show_overlay) {
        for (auto it = overlays[frame_nr].overlay.begin(); it != overlays[frame_nr].drawn; it++) {
            frame = (*it)->draw(frame);
        }
    }
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
}

void Overlay::set_text_settings(QString text, float font_scale) {
    current_string = text;
    current_font_scale = font_scale;
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
 * @brief Overlay::empty_undo_list
 * Clean the top end of the drawing list so you can't redo anymore
 * @param frame_nr
 */
void Overlay::empty_undo_list(int frame_nr) {
    overlays[frame_nr].overlay.erase(overlays[frame_nr].drawn, overlays[frame_nr].overlay.end());
}

/**
 * @brief Overlay::add_drawing
 * Add a drawing to the list of drawings
 * @param shape
 * @param frame_nr
 */
void Overlay::add_drawing(Shape* shape, int frame_nr) {
    if (shape->get_shape() == TEXT) {
        shape->set_text_size(cv::getTextSize(current_string.toStdString(), cv::FONT_HERSHEY_SIMPLEX, current_font_scale, shape->LINE_THICKNESS, &baseline));
    }
    overlays[frame_nr].overlay.push_back(shape);
    overlays[frame_nr].drawn = overlays[frame_nr].overlay.end();
}

void Overlay::get_drawing(QPoint pos, int frame_nr) {
    current_drawing = nullptr;
    for (auto shape : overlays[frame_nr].overlay) {
        if (point_in_drawing(pos, shape)) {
            qDebug() << "Found one";
            current_drawing = shape;
        }
    }
}

bool Overlay::point_in_drawing(QPoint pos, Shape *shape) {
    cv::Rect drawing = cv::Rect(shape->get_draw_start(), shape->get_draw_end());
    return drawing.contains(qpoint_to_point(pos));
}

/**
 * @brief Shape::qpoint_to_point
 * Converts QPoint to OpenCV Point.
 * @param pnt QPoint to be converted.
 * @return Returns converted Point.
 */
cv::Point Overlay::qpoint_to_point(QPoint pnt) {
    return cv::Point(pnt.x(), pnt.y());
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
                empty_undo_list(frame_nr);
                add_drawing(new Rectangle(current_colour, pos), frame_nr);
                break;
            case CIRCLE:
                empty_undo_list(frame_nr);
                add_drawing(new Circle(current_colour, pos), frame_nr);
                break;
            case LINE:
                empty_undo_list(frame_nr);
                add_drawing(new Line(current_colour, pos), frame_nr);
                break;
            case ARROW:
                empty_undo_list(frame_nr);
                add_drawing(new Arrow(current_colour, pos), frame_nr);
                break;
            case PEN:
                empty_undo_list(frame_nr);
                add_drawing(new Pen(current_colour, pos), frame_nr);
                break;
            case TEXT:
                empty_undo_list(frame_nr);
                add_drawing(new Text(current_colour, pos, current_string, current_font_scale), frame_nr);
                break;
            case HAND:
                get_drawing(pos, frame_nr);
                prev_point = pos;
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
    if (show_overlay && !overlays[frame_nr].overlay.empty()) {
        if (current_shape == HAND) {
            if (current_drawing == nullptr) return;
            qDebug() << "in hand";
            QPoint diff_point = pos - prev_point;
            current_drawing->move_shape(diff_point);
            prev_point = pos;
        } else if (current_shape == TEXT) {
            overlays[frame_nr].overlay.back()->update_text(pos);
        } else {
            // The last appended shape is the one we're currently drawing.
            overlays[frame_nr].overlay.back()->update_drawing_pos(pos);
        }
    }
}

std::map<int, FrameOverlay> Overlay::get_overlays() {
    return overlays;
}

void Overlay::set_overlays(std::map<int, FrameOverlay> new_overlays) {
    overlays = new_overlays;
}

/**
 * @brief Overlay::undo
 * Undo the drawings on the overlay, if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::undo(int frame_nr) {
    if (show_overlay && overlays[frame_nr].overlay.begin() != overlays[frame_nr].drawn) {
        overlays[frame_nr].drawn--;
    }
}

void Overlay::redo(int frame_nr) {
    if (show_overlay && overlays[frame_nr].overlay.end() != overlays[frame_nr].drawn) {
        overlays[frame_nr].drawn++;
    }
}

/**
 * @brief Overlay::clear
 * Clear the drawings on the overlay, if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::clear(int frame_nr) {
    if (show_overlay) {
        overlays[frame_nr].overlay.clear();
        overlays[frame_nr].drawn = overlays[frame_nr].overlay.end();
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
            add_drawing(shape, frame_nr);
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
        for (auto it = map_entry.second.overlay.begin(); it != map_entry.second.drawn; it ++) {  // Second member is the value, i.e. the drawings.
            QJsonObject json_shape;
            (*it)->write(json_shape);
            json_drawings.append(json_shape);
        }
        json_overlay["frame"] = map_entry.first; // First member is the key, i.e. the frame number.
        json_overlay["drawings"] = json_drawings;
        json_overlays.push_back(json_overlay);
    }
    json["overlays"] = json_overlays;
}

