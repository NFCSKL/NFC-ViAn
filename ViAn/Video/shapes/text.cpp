#include "text.h"

/**
 * @brief Text::Text
 */
Text::Text() : Shapes(SHAPES::TEXT) {
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
}

Text::~Text() {}

/**
 * @brief Text::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Text::draw(cv::Mat &frame) {
    cv::putText(frame, m_name.toStdString(), draw_start, cv::FONT_HERSHEY_SIMPLEX, font_scale,
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

void Text::set_font_scale(QPoint diff_point) {
    int diff_sum = diff_point.x() + diff_point.y();
    if (diff_sum > 0 && font_scale < FONT_SCALE_MAX) {
        font_scale += FONT_SCALE_STEP;
    } else if (diff_sum < 0 && font_scale > FONT_SCALE_MIN) {
        font_scale += -FONT_SCALE_STEP;
    }
}

double Text::get_font_scale() {
    return font_scale;
}

QString Text::get_name() {
    return m_name;
}

void Text::set_name(QString name) {
    m_name = name;
}

/**
 * @brief Text::write
 * @param json
 * Writes to a Json object.
 */
void Text::write(QJsonObject& json) {
    write_shape(json);
    json["name"] = m_name;
    json["font"] = font_scale;
}

/**
 * @brief Text::read
 * @param json
 * Reads from a Json object.
 */
void Text::read(const QJsonObject& json) {
    read_shape(json);
    this->m_name = json["name"].toString();
    this->font_scale = json["font"].toDouble();
}
