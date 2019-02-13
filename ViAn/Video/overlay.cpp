#include "overlay.h"

#include "shapes/arrow.h"
#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/pen.h"
#include "shapes/rectangle.h"
#include "shapes/text.h"
#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QDebug>
#include <QJsonArray>

/**
 * @brief Overlay::Overlay
 */
Overlay::Overlay() {}

Overlay::~Overlay() {
    emit clean_overlay();
    for (auto &vector : overlays) {
        for (Shapes* shape : vector.second) {
            delete shape;
        }
        vector.second.clear();
    }
    overlays.clear();
}

/**
 * @brief Overlay::draw_overlay
 * Draws the text drawings from an overlay on top of the specified frame.
 * @param img Frame to draw on
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::draw_overlay(cv::Mat &frame, int frame_nr, cv::Point anchor, double scale_factor, int angle, int width, int height) {
    if (show_overlay) {
        for (auto it = overlays[frame_nr].begin(); it != overlays[frame_nr].end(); it++) {
            // Only draw the text drawings
            if ((*it)->get_show() && (*it)->get_shape() == TEXT) {
                frame = (*it)->draw_scaled(frame, anchor, scale_factor, angle, width, height);
                Text* text = dynamic_cast<Text*>(*it);
                cv::Point p;
                if (angle == 90) {
                    p = cv::Point(text->get_draw_start().x - text->get_text_size().height,
                                                      text->get_draw_start().y - text->get_text_size().width);
                } else if (angle == 180) {
                    p = cv::Point(text->get_draw_start().x - text->get_text_size().width,
                                                      text->get_draw_start().y + text->get_text_size().height);
                } else if (angle == 270) {
                    p = cv::Point(text->get_draw_start().x + text->get_text_size().height,
                                                      text->get_draw_start().y + text->get_text_size().width);
                } else {
                    p = text->get_draw_end();
                }
                text->set_text_draw_end(p);
            }
        }
    }
    current_frame = frame_nr;
}

/**
 * @brief Overlay::draw_overlay_scaled
 * Draws the non-text drawings from an overlay on top of the specified frame.
 * These drawings are scaled to fit the Mat
 * @param img - Frame to draw on
 * @param frame_nr - Number of the frame currently shown in the video.
 * @param anchor - The top left coordinate of the zoomrect, used to scale the drawings
 * @param scale_factor - The zoom percent, used to scale the drawings
 */
void Overlay::draw_overlay_scaled(cv::Mat &frame, int frame_nr, cv::Point anchor, double scale_factor, int angle, int width, int height) {
    if (show_overlay) {
        for (auto it = overlays[frame_nr].begin(); it != overlays[frame_nr].end(); it++) {
            // Don't draw text drawings since they work a little different.
            // They can't be scaled as big and small as needed the same way as other drawings
            // They are drawing first and then scaled with the Mat
            if ((*it)->get_show() && (*it)->get_shape() != TEXT) {
                // The anchor and scale factor is used to scale the drawing
                frame = (*it)->draw_scaled(frame, anchor, scale_factor, angle, width, height);
            }
        }
    }
    current_frame = frame_nr;
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
 * @param s New tool to be set.
 */
void Overlay::set_tool(SHAPES s) {
    current_shape = s;
}

/**
 * @brief Overlay::create_text
 * Create a new text drawing
 * @param pos
 * @param frame
 */
void Overlay::create_text(QPoint pos, int frame) {
    Text* text = new Text(current_colour, pos, current_string, current_font_scale);
    add_drawing(text, frame);
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

int Overlay::get_current_frame() {
    return current_frame;
}

/**
 * @brief Overlay::get_empty_shape
 * @param shape_type
 * @return Returns a new shape of the given type.
 */
Shapes* Overlay::get_empty_shape(SHAPES shape_type) {
    switch (shape_type) {
        case RECTANGLE:
            return new Rectangle();
        case CIRCLE:
            return new Circle();
        case LINE:
            return new Line();
        case ARROW:
            return new Arrow();
        case PEN:
            return new Pen();
        case TEXT:
            return new Text();
        default:
            return nullptr;
    }
}

/**
 * @brief Overlay::add_drawing
 * Add a drawing to the list of drawings
 * @param shape
 * @param frame_nr
 */
void Overlay::add_drawing(Shapes* shape, int frame_nr) {
    if (shape->get_shape() == TEXT) {
        Text* temp_text = dynamic_cast<Text*>(shape);
        // If the text size is the default value set new one
        // Otherwise use the saved one.
        if (temp_text->get_text_size() == cv::Size(-1, -1)) {
            temp_text->set_text_size(cv::getTextSize(current_string.toStdString(), cv::FONT_HERSHEY_SIMPLEX, current_font_scale, Singleton::get_instance()->LINE_THICKNESS, &baseline));
        }
    }
    shape->set_frame(frame_nr);
    overlays[frame_nr].push_back(shape);
    emit new_drawing(shape, frame_nr);
    set_current_drawing(shape);
    m_unsaved_changes = true;
}

/**
 * @brief Overlay::get_drawing
 * Set the clicked drawing to current drawing
 * @param pos
 * @param frame_nr
 */
void Overlay::get_drawing(QPoint pos, int frame_nr) {
    current_drawing = nullptr;
    for (auto shape : overlays[frame_nr]) {
        if (point_in_drawing(pos, shape)) {
            current_drawing = shape;
        }
    }
    emit select_current(current_drawing, frame_nr);
}

void Overlay::set_current_drawing(Shapes *shape) {
    current_drawing = shape;
}

Shapes* Overlay::get_current_drawing() {
    return current_drawing;
}

bool Overlay::get_show_overlay() {
    return show_overlay;
}

void Overlay::update_text(QString text, Shapes* shape) {
    if (shape->get_shape() == TEXT) {
        shape->set_name(text);
        Text* temp_text = dynamic_cast<Text*>(shape);
        double font_scale = temp_text->get_font_scale();
        temp_text->set_text_size(cv::getTextSize(text.toStdString(), cv::FONT_HERSHEY_SIMPLEX, font_scale, Singleton::get_instance()->LINE_THICKNESS, &baseline));
        temp_text->update_text_draw_end();
    }
}

/**
 * @brief Overlay::point_in_drawing
 * @param pos
 * @param shape
 * @return true if the point pos is in the hidden rect of drawing shape
 */
bool Overlay::point_in_drawing(QPoint pos, Shapes *shape) {
    cv::Rect drawing;
    int tl_x, tl_y, br_x, br_y;
    if (shape->get_shape() == PEN) {
        Pen* current = dynamic_cast<Pen*>(shape);
        drawing = cv::boundingRect(current->get_points());
        tl_x = drawing.tl().x;
        tl_y = drawing.tl().y;
        br_x = drawing.br().x;
        br_y = drawing.br().y;
    } else if (shape->get_shape() == TEXT) {
        Text* current = dynamic_cast<Text*>(shape);
        tl_x = current->get_draw_start().x;
        tl_y = current->get_draw_start().y;
        br_x = current->get_text_draw_end().x;
        br_y = current->get_text_draw_end().y;
    } else {
        tl_x = shape->get_draw_start().x;
        tl_y = shape->get_draw_start().y;
        br_x = shape->get_draw_end().x;
        br_y = shape->get_draw_end().y;
    }

    if (tl_y - br_y <= DRAW_RECT_MIN && tl_y - br_y >= -DRAW_RECT_MIN) {
        tl_y += -DRAW_RECT_MARGIN;
        br_y += DRAW_RECT_MARGIN;
    }

    if (tl_x - br_x <= DRAW_RECT_MIN && tl_x - br_x >= -DRAW_RECT_MIN) {
        tl_x += -DRAW_RECT_MARGIN;
        br_x += DRAW_RECT_MARGIN;
    }
    drawing = cv::Rect(cv::Point(tl_x, tl_y), cv::Point(br_x, br_y));
    return shape->get_show() && drawing.contains(Utility::from_qpoint(pos));
}

void Overlay::mouse_double_clicked(QPoint pos, int frame_nr) {
    switch (current_shape) {
    case EDIT:
        for (auto shape : overlays[frame_nr]) {
            if (point_in_drawing(pos, shape)) {
                mouse_pressed(pos, frame_nr, false);
                return;
            }
        }
        emit set_tool_zoom();
        break;
    case ZOOM:
        for (auto shape : overlays[frame_nr]) {
            if (point_in_drawing(pos, shape)) {
                get_drawing(pos, frame_nr);
                emit set_tool_edit();
                set_tool(EDIT);
                prev_point = pos;
                mouse_pressed(pos, frame_nr, false);
                break;
            }
        }
        break;
    default:
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
void Overlay::mouse_pressed(QPoint pos, int frame_nr, bool right_click) {
    if (show_overlay) {
        if (current_shape == EDIT) {
            edit = true;
            prev_point = pos;
            m_right_click = right_click;
            if (right_click) {
                if (current_drawing && current_drawing->get_frame() == frame_nr) {
                    current_drawing->set_anchor(pos);
                }
                return;
            }

            if (current_drawing && current_drawing->get_frame() == frame_nr
                    && point_in_drawing(pos, current_drawing)) {
                return;
            } else {
                get_drawing(pos, frame_nr);
            }
            return;
        }

        if (right_click) {
            emit set_tool_zoom();
            return;
        }
        switch (current_shape) {
        case RECTANGLE:
            add_drawing(new Rectangle(current_colour, pos), frame_nr);
            drawing = true;
            break;
        case CIRCLE:
            add_drawing(new Circle(current_colour, pos), frame_nr);
            drawing = true;
            break;
        case LINE:
            add_drawing(new Line(current_colour, pos), frame_nr);
            drawing = true;
            break;
        case ARROW:
            add_drawing(new Arrow(current_colour, pos), frame_nr);
            drawing = true;
            break;
        case PEN:
            add_drawing(new Pen(current_colour, pos), frame_nr);
            drawing = true;
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
void Overlay::mouse_released(QPoint pos, int frame_nr, bool right_click) {
    Q_UNUSED(pos)
    Q_UNUSED(frame_nr)
    edit = false;
    if (change_tool) {
        emit set_tool_zoom();
        change_tool = false;
        return;
    }
    if (drawing) {
        emit set_tool_edit();
        drawing = false;
        return;
    }
    m_right_click = right_click;
}

/**
 * @brief Overlay::mouse_moved
 * Updates drawing on the overlay when the mouse is
 * moved, if the overlay is visible.
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::mouse_moved(QPoint pos, int frame_nr, bool shift, bool ctrl) {
    if (change_tool) return;
    update_drawing_position(pos, frame_nr, shift, ctrl);
}

/**
 * @brief Overlay::mouse_scroll
 * Updates the thickness of the lines of the current drawing
 * when scrolling the mouse wheel.
 * @param pos Mouse wheel delta.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::mouse_scroll(QPoint pos, int frame_nr) {
    if (!current_drawing) return;
    if (current_drawing->get_shape() == TEXT) {
        Text* temp_text = dynamic_cast<Text*>(current_drawing);
        double font_scale = temp_text->set_font_scale(pos);
        temp_text->set_text_size(cv::getTextSize(temp_text->get_name().toStdString(), cv::FONT_HERSHEY_SIMPLEX, font_scale, Singleton::get_instance()->LINE_THICKNESS, &baseline));
        temp_text->update_text_draw_end();
        m_unsaved_changes = true;
        return;
    }
    if (current_drawing->get_frame() == frame_nr && show_overlay) {
        current_drawing->set_thickness(pos);
        m_unsaved_changes = true;
    }
}

/**
 * @brief Overlay::update_drawing_position
 * Updates the position of the end point of the shape currently being drawn
 * @param pos Mouse coordinates on the frame.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::update_drawing_position(QPoint pos, int frame_nr, bool shift, bool ctrl) {
    // Only update the overlay is is exists and is currently being shown
    if (show_overlay && !overlays[frame_nr].empty()) {
        if (current_shape == EDIT && edit) {
            if (!current_drawing || current_drawing->get_frame() != frame_nr) return;
            if (m_right_click && current_drawing->get_shape() == TEXT) {
                QPoint diff_point = pos - prev_point;
                Text* temp_text = dynamic_cast<Text*>(current_drawing);
                double font_scale = temp_text->set_font_scale(diff_point);
                temp_text->set_text_size(cv::getTextSize(temp_text->get_name().toStdString(), cv::FONT_HERSHEY_SIMPLEX, font_scale, Singleton::get_instance()->LINE_THICKNESS, &baseline));
                temp_text->update_text_draw_end();
                prev_point = pos;
                m_unsaved_changes = true;
                return;
            }
            else if (m_right_click) {
                QPoint diff_point = pos - prev_point;
                current_drawing->edit_shape(diff_point);
                prev_point = pos;
                m_unsaved_changes = true;
                return;
            }
            QPoint diff_point = pos - prev_point;
            current_drawing->move_shape(diff_point);
            prev_point = pos;
        } else if (current_shape == TEXT) {
            dynamic_cast<Text*>(overlays[frame_nr].back())->update_text_pos(pos);
        } else if (current_shape != ZOOM && current_shape != EDIT){
            if (current_shape != PEN && shift) {
                // When the shift modifier is used draw a symmetric drawing
                // It's not possible with the pen tool.
                int x = pos.x() - overlays[frame_nr].back()->get_draw_start().x;
                int y = pos.y() - overlays[frame_nr].back()->get_draw_start().y;

                if (x >= 0 && y >= 0) {
                    overlays[frame_nr].back()->update_drawing_sym(std::max(x, y), std::max(x, y));
                } else if (x <= 0 && y <= 0) {
                    overlays[frame_nr].back()->update_drawing_sym(std::min(x, y), std::min(x, y));
                } else if (x >= 0 && y <= 0) {
                    int change = std::max(abs(x), abs(y));
                    overlays[frame_nr].back()->update_drawing_sym(change, -change);
                } else if (x <= 0 && y >= 0) {
                    int change = std::max(abs(x), abs(y));
                    overlays[frame_nr].back()->update_drawing_sym(-change, change);
                }
                m_unsaved_changes = true;
                return;
            } else if (ctrl && (current_shape == LINE || current_shape == ARROW)) {
                int x = pos.x() - overlays[frame_nr].back()->get_draw_start().x;
                int y = pos.y() - overlays[frame_nr].back()->get_draw_start().y;

                if (x >= abs(y)) {
                    overlays[frame_nr].back()->update_drawing_sym(x, 0);
                } else if (-x >= abs(y)) {
                    overlays[frame_nr].back()->update_drawing_sym(x, 0);
                } else if (y >= abs(x)) {
                    overlays[frame_nr].back()->update_drawing_sym(0, y);
                } else if (-y >= abs(x)) {
                    overlays[frame_nr].back()->update_drawing_sym(0, y);
                }
                m_unsaved_changes = true;
                return;
            }
            // The last appended shape is the one we're currently drawing.
            overlays[frame_nr].back()->update_drawing_pos(pos);
        }
        m_unsaved_changes = true;
    }
}

std::map<int,std::vector<Shapes*>> Overlay::get_overlays() {
    return overlays;
}

void Overlay::set_overlays(std::map<int, std::vector<Shapes*>> new_overlays) {
    overlays = new_overlays;
    m_unsaved_changes = true;
}

void Overlay::set_overlay_changed() {
    m_unsaved_changes = true;
}

bool Overlay::is_saved() const{
    return !m_unsaved_changes;
}

/**
 * @brief Overlay::clear
 * Clear the drawings on the overlay
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::clear(int frame_nr) {
    if (show_overlay) {
        overlays[frame_nr].clear();
        m_unsaved_changes = true;
    }
}

/**
 * @brief Overlay::delete_drawing
 * Delete the current drawing if the overlay is visible.
 * @param frame_nr Number of the frame currently shown in the video.
 */
void Overlay::delete_drawing(Shapes* shape) {
    Shapes* drawing;
    if (shape == nullptr) {
        drawing = current_drawing;
    } else {
        drawing = shape;
    }
    auto it = std::find(overlays[drawing->get_frame()].begin(), overlays[drawing->get_frame()].end(), drawing);

    if (drawing == current_drawing) current_drawing = nullptr;

    if (it != overlays[drawing->get_frame()].end()) {
        overlays[drawing->get_frame()].erase(it);
    }
    delete drawing;
    m_unsaved_changes = true;
}

/**
 * @brief Overlay::remove_frame
 * Used when a frame is removed from a sequence.
 * Updates the overlays map with the new frame numbers
 * @param frame
 */
void Overlay::remove_frame(int frame) {
    std::map<int, std::vector<Shapes*>> new_overlays;
    bool found = false;
    for (auto node : overlays) {
        if (node.first == frame && !found) {
            found = true;
            node.second.clear();
        } else if (found && node.first > frame) {
            new_overlays[node.first-1] = node.second;
        } else {
            new_overlays[node.first] = node.second;
        }
    }
    set_overlays(new_overlays);
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
            Shapes* shape = get_empty_shape(shape_t);
            shape->read(json_shape);
            add_drawing(shape, frame_nr);
            set_current_drawing(nullptr);
        }
    }
    m_unsaved_changes = false;
}

/**
 * @brief Overlay::write
 * @param json
 * Writes the overlay to a Json object.
 */
void Overlay::write(QJsonObject& json) {
    QJsonArray json_overlays;
    for (auto const& map_entry : overlays) {
        if (!map_entry.second.empty()) {
            QJsonObject json_overlay;
            QJsonArray json_drawings;
            for (auto it = map_entry.second.begin(); it != map_entry.second.end(); it ++) {  // Second member is the value, i.e. the drawings.
                QJsonObject json_shape;
                (*it)->write(json_shape);
                json_drawings.append(json_shape);
            }
            json_overlay["frame"] = map_entry.first; // First member is the key, i.e. the frame number.
            json_overlay["drawings"] = json_drawings;
            json_overlays.push_back(json_overlay);
        }
    }
    json["overlays"] = json_overlays;
    m_unsaved_changes = false;
}
