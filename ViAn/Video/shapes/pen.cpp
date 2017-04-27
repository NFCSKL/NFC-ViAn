#include "pen.h"

/**
 * @brief Pen::Pen
 */
Pen::Pen() : Shape(SHAPES::PEN) {
}

/**
 * @brief Pen::Pen
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Pen::Pen(QColor col, QPoint pos) : Shape(SHAPES::PEN, col, pos) {
}

/**
 * @brief Pen::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Pen::draw(cv::Mat &frame) {
    for (std::pair<cv::Point, cv::Point> line : lines) {
        cv::line(frame, line.first, line.second, colour, LINE_THICKNESS);
    }
    return frame;
}

/**
 * @brief Pen::handle_new_pos
 * Function to handle the new position of the mouse.
 * Does not need to store the new position.
 * @param pos
 */
void Pen::handle_new_pos(QPoint pos) {
    std::pair<cv::Point, cv::Point> line(draw_end, qpoint_to_point(pos));
    lines.push_back(line);
}

/**
 * @brief Pen::write
 * @param json
 * Writes to a Json object.
 */
void Pen::write(QJsonObject& json) {
    write_shape(json);
    QJsonArray json_lines;
    for (std::pair<cv::Point, cv::Point> line : lines) {
        QJsonObject json_line;
        json_line["p1x"] = line.first.x;
        json_line["p1y"] = line.first.y;
        json_line["p2x"] = line.second.x;
        json_line["p2y"] = line.second.y;
        json_lines.append(json_line);
    }
    json["lines"] = json_lines;
}

/**
 * @brief Pen::read
 * @param json
 * Reads from a Json object.
 */
void Pen::read(const QJsonObject& json) {
    read_shape(json);
    QJsonArray json_lines = json["lines"].toArray();
    for(int i = 0; i != json_lines.size(); i++) {
        QJsonObject json_line = json_lines[i].toObject();
        cv::Point start;
        start.x = json_line["p1x"].toInt();
        start.y = json_line["p1y"].toInt();
        cv::Point end;
        end.x = json_line["p2x"].toInt();
        end.y = json_line["p2y"].toInt();
        std::pair<cv::Point, cv::Point> line(start, end);
        lines.push_back(line);
    }
}
