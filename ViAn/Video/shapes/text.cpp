#include "text.h"

/**
 * @brief Text::Text
 */
Text::Text() : Shape(SHAPES::TEXT) {
    string = QString();
    font_scale = 0;
}

/**
 * @brief Text::Text
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 * @param strng String to be displayed
 * @param fnt_scl Font scale of the string
 */
Text::Text(QColor col, QPoint pos, QString strng, double fnt_scl) : Shape(SHAPES::TEXT, col, pos) {
    string = strng;
    font_scale = fnt_scl;
    text_size = cv::getTextSize(string.toStdString(), cv::FONT_HERSHEY_SIMPLEX, font_scale, thickness, &baseline);
}

/**
 * @brief Text::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Text::draw(cv::Mat &frame) {
    cv::putText(frame, string.toStdString(), draw_start, cv::FONT_HERSHEY_SIMPLEX, font_scale,
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

/**
 * @brief Text::write
 * @param json
 * Writes to a Json object.
 */
void Text::write(QJsonObject& json) {
    write_shape(json);
    json["text"] = string;
    json["font"] = font_scale;
}

/**
 * @brief Text::read
 * @param json
 * Reads from a Json object.
 */
void Text::read(const QJsonObject& json) {
    read_shape(json);
    this->string = json["text"].toString();
    this->font_scale = json["font"].toDouble();
}
