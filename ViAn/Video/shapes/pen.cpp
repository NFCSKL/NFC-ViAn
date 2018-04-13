#include "pen.h"
#include <QDebug>

/**
 * @brief Pen::Pen
 */
Pen::Pen() : Shapes(SHAPES::PEN) {
}

/**
 * @brief Pen::Pen
 * @param col Colour of the new object
 * @param pos Starting point for the new object
 */
Pen::Pen(QColor col, QPoint pos) : Shapes(SHAPES::PEN, col, pos) {
    points.push_back(qpoint_to_point(pos));
}

Pen::~Pen() {}

/**
 * @brief Pen::draw
 * Draws the object on top of the specified frame.
 * @param frame Frame to draw on.
 * @return Returns the frame with drawing.
 */
cv::Mat Pen::draw(cv::Mat &frame) {
    cv::Point p(-1, -1);
    for (auto point : points) {
        if (p.x < 0 || p.y < 0) {
            p = point;
            continue;
        }
        cv::line(frame, p, point, color, thickness);
        p = point;
    }
    return frame;
    
    
//    for (std::pair<cv::Point, cv::Point> line : lines) {
//        cv::line(frame, line.first, line.second, color, thickness);
//    }
//    return frame;
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
    points.push_back(qpoint_to_point(pos));
}

void Pen::move_shape(QPoint diff_point) {
    std::vector<cv::Point> new_points;
    for (cv::Point point : points) {
        new_points.push_back(point + qpoint_to_point(diff_point));
    }
    points = new_points;
}

QString Pen::get_name() {
    return m_name;
}

void Pen::set_name(QString name) {
    m_name = name;
}

std::vector<cv::Point> Pen::get_points() {
    return points;
}

/**
 * @brief Pen::write
 * @param json
 * Writes to a Json object.
 */
void Pen::write(QJsonObject& json) {
    write_shape(json);
    QJsonArray json_points;
    for (cv::Point point : points) {
        QJsonObject json_point;
        json_point["px"] = point.x;
        json_point["py"] = point.y;
        json_points.append(json_point);
    }
    json["points"] = json_points;

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
    json["name"] = m_name;
}

/**
 * @brief Pen::read
 * @param json
 * Reads from a Json object.
 */
void Pen::read(const QJsonObject& json) {
    read_shape(json);
    QJsonArray json_points = json["points"].toArray();
    for (int i = 0; i != json_points.size(); i++) {
        QJsonObject json_point = json_points[i].toObject();
        cv::Point p;
        p.x = json_point["px"].toInt();
        p.y = json_point["py"].toInt();
        points.push_back(p);
    }



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
    m_name = json["name"].toString();
}
