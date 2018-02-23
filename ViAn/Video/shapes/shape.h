#ifndef SHAPES_H
#define SHAPES_H

#include <QImage>
#include <QJsonObject>
#include <QJsonArray>
#include <qpainter.h>
#include <algorithm>

#include "opencv2/opencv.hpp"

enum SHAPES {NONE, RECTANGLE, CIRCLE, LINE, ARROW, PEN, TEXT, HAND, ZOOM, MOVE, ANALYSIS_BOX};

class Shape {

public:
    Shape(SHAPES s);
    Shape(SHAPES s, QColor col, QPoint pos);
    void update_drawing_pos(QPoint pos);
    void update_text(QPoint pos);
    void move_shape(QPoint p);
    virtual void handle_new_pos(QPoint pos) = 0;
    virtual cv::Mat draw(cv::Mat &frame) = 0;

    virtual void read(const QJsonObject& json) = 0;
    virtual void write(QJsonObject& json) = 0;

    static cv::Scalar qcolor_to_scalar(QColor col);
    static cv::Point qpoint_to_point(QPoint pnt);

    static const int LINE_THICKNESS = 2; // Constant used for the thickness of the drawn shapes.
    static constexpr double ALPHA = 0.6; // Costant used for the opacity.

    cv::Point get_draw_start();
    cv::Point get_draw_end();
    SHAPES get_shape();
    void set_text_size(cv::Size size);
    void invert_color();

protected:
    SHAPES shape;
    cv::Scalar color;
    cv::Point draw_start;
    cv::Point draw_end;
    cv::Size text_size;

    void write_shape(QJsonObject& json);
    void read_shape(const QJsonObject& json);
};

#endif // SHAPES_H
