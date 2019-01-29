#include "text.h"

#include "constants.h"
#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <QPoint>

/**
 * @brief Text::Text
 */
Text::Text() : Shapes(SHAPES::TEXT) {
    m_name = "Text";
    font_scale = 0;
}

/**
 * @brief Text::Text
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 * @param strng String to be displayed
 * @param fnt_scl Font scale of the string
 */
Text::Text(QColor col, QPoint pos, QString strng, double fnt_scl) : Shapes(SHAPES::TEXT, col, pos) {
    set_name(strng);
    font_scale = fnt_scl;
    text_size = cv::getTextSize(m_name.toStdString(), cv::FONT_HERSHEY_SIMPLEX, font_scale, thickness, &baseline);
    cv::Point bl = cv::Point(draw_start.x-text_size.width/2, draw_start.y + text_size.height/2);
    draw_end = cv::Point(draw_start.x + text_size.width/2, draw_start.y-text_size.height/2);
    draw_start = bl;
}

Text::~Text() {}

cv::Mat Text::draw_scaled(cv::Mat &frame, cv::Point anchor, double scale_factor, int angle, int width, int height) {
    Q_UNUSED(anchor) Q_UNUSED(scale_factor)
    QPoint rot_start = Utility::rotate(Utility::from_cvpoint(draw_start), angle, width, height);
    cv::putText(frame, m_name.toStdString(), Utility::from_qpoint(rot_start), cv::FONT_HERSHEY_SIMPLEX, font_scale,
                color, thickness);
    return frame;
}

/**
 * @brief Text::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Text::handle_new_pos(QPoint pos) {
    Q_UNUSED( pos )
}

double Text::set_font_scale(QPoint diff_point) {
    int diff_sum = diff_point.x() + diff_point.y();
    if (diff_sum > 0 && font_scale < Constants::FONT_SCALE_MAX) {
        font_scale += Constants::FONT_SCALE_STEP;
    } else if (diff_sum < 0 && font_scale > Constants::FONT_SCALE_MIN) {
        font_scale += -Constants::FONT_SCALE_STEP;
    }
    return font_scale;
}

double Text::get_font_scale() {
    return font_scale;
}

cv::Size Text::get_text_size() {
    return text_size;
}

void Text::set_text_size(cv::Size size) {
    text_size = size;
}

/**
 * @brief Text::update_text_pos
 * Updates the start and end point of the text-drawing
 * @param pos
 */
void Text::update_text_pos(QPoint pos) {
    draw_start = Utility::from_qpoint(pos);
    cv::Point p(draw_start.x + text_size.width, draw_start.y - text_size.height);
    draw_end = p;
}

void Text::update_text_draw_end() {
    cv::Point p(draw_start.x + text_size.width, draw_start.y - text_size.height);
    draw_end = p;
}

cv::Point Text::get_text_draw_end() {
    return text_draw_end;
}

void Text::set_text_draw_end(cv::Point pos) {
    text_draw_end = pos;
}

/**
 * @brief Text::write
 * @param json
 * Writes to a Json object.
 */
void Text::write(QJsonObject& json) {
    write_shape(json);
    json["font"] = font_scale;
}

/**
 * @brief Text::read
 * @param json
 * Reads from a Json object.
 */
void Text::read(const QJsonObject& json) {
    read_shape(json);
    font_scale = json["font"].toDouble();
    text_size = cv::Size(draw_end.x - draw_start.x, draw_start.y - draw_end.y);
}
