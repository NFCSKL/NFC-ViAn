#include "shapes.h"

#include <QDebug>
#include <QPoint>


/**
 * @brief Shape::Shape
 * @param s
 */
Shapes::Shapes(SHAPES s) {
    shape = s;
    color = cv::Scalar();
    q_color = QColor();
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
    q_color = col;
    draw_start = qpoint_to_point(pos);
    draw_end = qpoint_to_point(pos);
}

Shapes::~Shapes() {}

/**
 * @brief Shapes::set_anchor
 * @param pos
 * Set the anchor to draw_start or draw_end
 * This is used in edit shape to know which point to edit
 */
void Shapes::set_anchor(QPoint pos) {
    QPoint start_point = QPoint(draw_start.x, draw_start.y) - pos;
    QPoint end_point = QPoint(draw_end.x, draw_end.y) - pos;
    if (start_point.manhattanLength() < end_point.manhattanLength()) {
        anchor = true;
    } else {
        anchor = false;
    }
}

/**
 * @brief Shapes::edit_shape
 * @param diff_point
 * Edits the shape, which point is edited is based on the anchor
 */
void Shapes::edit_shape(QPoint diff_point) {
    (anchor) ? (draw_start += qpoint_to_point(diff_point)) : draw_end += qpoint_to_point(diff_point);
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

void Shapes::update_drawing_sym(int dx, int dy) {
    draw_end = cv::Point(draw_start.x + dx, draw_start.y + dy);
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

void Shapes::update_text_draw_end() {
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

void Shapes::set_draw_end(cv::Point p) {
    draw_end = p;
}

void Shapes::set_draw_start(cv::Point p) {
    draw_start = p;
}

/**
 * @brief Shape::get_shape
 * @return shape
 */
SHAPES Shapes::get_shape() {
    return shape;
}

QColor Shapes::get_color() {
    return q_color;
}

void Shapes::set_color(QColor col) {
    q_color = col;
    color = qcolor_to_scalar(col);
}

cv::Size Shapes::get_text_size() {
    return text_size;
}

void Shapes::set_text_size(cv::Size size) {
    text_size = size;
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

void Shapes::set_thickness(int value) {
    thickness = value;
}

int Shapes::get_thickness() {
    return thickness;
}

void Shapes::set_frame(int frame_nr) {
    frame = frame_nr;
}

int Shapes::get_frame() {
    return frame;
}

void Shapes::set_name(QString name) {
    m_name = name;
}

QString Shapes::get_name() {
    return m_name;
}

bool Shapes::toggle_show() {
    show = !show;
    return show;
}

bool Shapes::get_show() {
    return show;
}

/**
 * @brief Shape::read_shape
 * @param json
 * Reads a shape from a Json object.
 */
void Shapes::read_shape(const QJsonObject& json){
    int shape_i = json["shape"].toInt();
    shape = static_cast<SHAPES>(shape_i);
    color[0] = json["b"].toInt();
    color[1] = json["g"].toInt();
    color[2] = json["r"].toInt();
    q_color = QColor(color[2], color[1], color[0]);
    draw_start.x = json["p1x"].toInt();
    draw_start.y = json["p1y"].toInt();
    draw_end.x = json["p2x"].toInt();
    draw_end.y = json["p2y"].toInt();
    frame = json["frame"].toInt();
    show = json["show"].toBool();
}

/**
 * @brief Shape::write_shape
 * @param json
 * Writes a shape to a Json object.
 */
void Shapes::write_shape(QJsonObject& json){
    json["shape"] = shape;
    json["b"] = color[0];
    json["g"] = color[1];
    json["r"] = color[2];
    json["p1x"] = draw_start.x;
    json["p1y"] = draw_start.y;
    json["p2x"] = draw_end.x;
    json["p2y"] = draw_end.y;
    json["frame"] = frame;
    json["show"] = show;
}
