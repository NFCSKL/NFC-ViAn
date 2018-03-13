#include "shapes.h"
#include <iostream>
#include <QDebug>

/**
 * @brief Shape::Shape
 * @param s
 */
Shapes::Shapes(SHAPES s) {
    shape = s;
    color = cv::Scalar();
    draw_start = cv::Point();
    draw_end = cv::Point();
}

/**
 * @brief Shape::Shape
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Shapes::Shapes(SHAPES s, QColor col, QPoint pos) {
    shape = s;
    color = qcolor_to_scalar(col);
    draw_start = qpoint_to_point(pos);
    draw_end = qpoint_to_point(pos);
}

void Shapes::edit_shape(QPoint diff_point, QPoint pos) {
    QPoint start_point = QPoint(draw_start.x, draw_start.y) - pos;
    QPoint end_point = QPoint(draw_end.x, draw_end.y) - pos;
    if (start_point.manhattanLength() < end_point.manhattanLength()) {
        draw_start += qpoint_to_point(diff_point);
    } else {
        draw_end += qpoint_to_point(diff_point);
    }
}

/**
 * @brief Shape::update_drawing_pos
 * Updates the position of the end point of the drawing
 * @param pos
 */
void Shapes::update_drawing_pos(QPoint pos) {
    // Call handle_new_pos first because it might need the old draw_end value. Only in pen
    handle_new_pos(pos);
    draw_end = qpoint_to_point(pos);
}

/**
 * @brief Shape::update_text_pos
 * Updates the start and end point of the text-drawing
 * @param pos
 */
void Shapes::update_text_pos(QPoint pos) {
    draw_start = qpoint_to_point(pos);
    cv::Point p(draw_start.x + text_size.width, draw_start.y - text_size.height);
    draw_end = p;
}

/**
 * @brief Shape::move_shape
 * @param diff_point
 * Adds the diff_pointer to the shape and therefore moves it that much
 */
void Shapes::move_shape(QPoint diff_point) {
    draw_start += qpoint_to_point(diff_point);
    draw_end += qpoint_to_point(diff_point);
}

/**
 * @brief Shape::qcolor_to_scalar
 * Converts QColor to OpenCV Scalar.
 * @param col QColor to be converted.
 * @return Returns converted colour.
 */
cv::Scalar Shapes::qcolor_to_scalar(QColor col) {
    int r,g,b;
    col.getRgb(&r, &g, &b);
    return cv::Scalar(b,g,r); // swap RGB-->BGR
}

/**
 * @brief Shape::qpoint_to_point
 * Converts QPoint to OpenCV Point.
 * @param pnt QPoint to be converted.
 * @return Returns converted Point.
 */
cv::Point Shapes::qpoint_to_point(QPoint pnt) {
    return cv::Point(pnt.x(), pnt.y());
}

/**
 * @brief Shape::get_draw_start
 * @return draw_start
 */
cv::Point Shapes::get_draw_start() {
    return draw_start;
}

/**
 * @brief Shape::get_draw_end
 * @return draw_end
 */
cv::Point Shapes::get_draw_end() {
    return draw_end;
}

/**
 * @brief Shape::get_shape
 * @return shape
 */
SHAPES Shapes::get_shape() {
    return shape;
}

void Shapes::set_text_size(cv::Size size) {
    text_size = size;
}

/**
 * @brief Shape::invert_color
 * Inverts the color by subtracting its RGB value from 255
 */
void Shapes::invert_color() {
    color = cv::Scalar::all(RGB_MAX) - color;
    inverted = !inverted;
}

/**
 * @brief Shape::set_thickness
 * Sets the shape's thickness with checks so it can't go into the negatives.
 * Circle and rectangle allow one step of negative which will fill the shape.
 * @param pos
 */
void Shapes::set_thickness(QPoint pos) {
    int new_thick = thickness + pos.y();
    if ((shape == CIRCLE || shape == RECTANGLE) && new_thick <= -2) return;
    if (!(shape == CIRCLE || shape == RECTANGLE) && new_thick <= -1) return;
    thickness = new_thick;
}

void Shapes::set_frame(int frame_nr) {
    frame = frame_nr;
}

int Shapes::get_frame() {
    return frame;
}

void Shapes::set_name(std::string name) {
    m_name = name;
}

std::string Shapes::get_name() {
    return m_name;
}

/**
 * @brief Shape::read_shape
 * @param json
 * Reads a shape from a Json object.
 */
void Shapes::read_shape(const QJsonObject& json){
    int shape_i = json["shape"].toInt();
    this->shape = static_cast<SHAPES>(shape_i);
    this->color[0] = json["b"].toInt();
    this->color[1] = json["g"].toInt();
    this->color[2] = json["r"].toInt();
    this->draw_start.x = json["p1x"].toInt();
    this->draw_start.y = json["p1y"].toInt();
    this->draw_end.x = json["p2x"].toInt();
    this->draw_end.y = json["p2y"].toInt();
    this->frame = json["frame"].toInt();
    this->m_name = json["name"].toString().toStdString();
}

/**
 * @brief Shape::write_shape
 * @param json
 * Writes a shape to a Json object.
 * If the shape is the current_drawing, save its original color instead of the inverted
 */
void Shapes::write_shape(QJsonObject& json){
    json["shape"] = this->shape;
    if (inverted) {
        json["b"] = RGB_MAX - this->color[0];
        json["g"] = RGB_MAX - this->color[1];
        json["r"] = RGB_MAX - this->color[2];
    } else {
        json["b"] = this->color[0];
        json["g"] = this->color[1];
        json["r"] = this->color[2];
    }
    json["p1x"] = this->draw_start.x;
    json["p1y"] = this->draw_start.y;
    json["p2x"] = this->draw_end.x;
    json["p2y"] = this->draw_end.y;
    json["frame"] = this->frame;
    json["name"] = QString::fromStdString(this->m_name);
}

