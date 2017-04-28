#include "shape.h"
#include <iostream>

/**
 * @brief Shape::Shape
 * @param s
 */
Shape::Shape(SHAPES s) {
    shape = s;
    colour = cv::Scalar();
    draw_start = cv::Point();
    draw_end = cv::Point();
}

/**
 * @brief Shape::Shape
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Shape::Shape(SHAPES s, QColor col, QPoint pos) {
    shape = s;
    colour = qcolor_to_scalar(col);
    draw_start = qpoint_to_point(pos);
    draw_end = qpoint_to_point(pos);
}

/**
 * @brief Shape::update_drawing_pos
 * Updates the position of the end point of the drawing
 * @param pos
 */
void Shape::update_drawing_pos(QPoint pos) {
    // Call handle_new_pos first because it might need the old draw_end value.
    handle_new_pos(pos);
    draw_end = qpoint_to_point(pos);
}

/**
 * @brief Shape::qcolor_to_scalar
 * Converts QColor to OpenCV Scalar.
 * @param col QColor to be converted.
 * @return Returns converted colour.
 */
cv::Scalar Shape::qcolor_to_scalar(QColor col) {
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
cv::Point Shape::qpoint_to_point(QPoint pnt) {
    return cv::Point(pnt.x(), pnt.y());
}

/**
 * @brief Shape::read_shape
 * @param json
 * Reads a shape from a Json object.
 */
void Shape::read_shape(const QJsonObject& json){
    int shape_i = json["shape"].toInt();
    this->shape = static_cast<SHAPES>(shape_i);
    this->colour[0] = json["b"].toInt();
    this->colour[1] = json["g"].toInt();
    this->colour[2] = json["r"].toInt();
    this->draw_start.x = json["p1x"].toInt();
    this->draw_start.y = json["p1y"].toInt();
    this->draw_end.x = json["p2x"].toInt();
    this->draw_end.y = json["p2y"].toInt();
}

/**
 * @brief Shape::write_shape
 * @param json
 * Writes a shape to a Json object.
 */
void Shape::write_shape(QJsonObject& json){
    json["shape"] = this->shape;
    json["b"] = this->colour[0];
    json["g"] = this->colour[1];
    json["r"] = this->colour[2];
    json["p1x"] = this->draw_start.x;
    json["p1y"] = this->draw_start.y;
    json["p2x"] = this->draw_end.x;
    json["p2y"] = this->draw_end.y;
}

