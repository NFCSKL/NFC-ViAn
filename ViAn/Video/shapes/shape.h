#ifndef SHAPES_H
#define SHAPES_H

#include <QImage>
#include <qpainter.h>
#include <algorithm>

#include "opencv2/opencv.hpp"

enum SHAPES {RECTANGLE, CIRCLE, LINE, ARROW, PEN, TEXT};

class Shape {

public:
    Shape(QColor col, QPoint pos);
    void update_drawing_pos(QPoint pos);
    virtual void handle_new_pos(QPoint pos) = 0;
    virtual cv::Mat draw(cv::Mat &frame) = 0;
protected:
    int LINE_THICKNESS = 2;

    static cv::Scalar qcolor_to_scalar(QColor col);
    static cv::Point qpoint_to_point(QPoint pnt);

    cv::Scalar colour;
    cv::Point draw_start;
    cv::Point draw_end;
};

#endif // SHAPES_H
