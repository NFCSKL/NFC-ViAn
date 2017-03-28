#ifndef SHAPES_H
#define SHAPES_H

#include <QImage>
#include <qpainter.h>
#include <algorithm>

#include "opencv2/opencv.hpp"

enum SHAPES {RECTANGLE, CIRCLE, LINE, ARROW, PEN, TEXT};

class shape {

public:
    shape(QColor col, QPoint pos);
    void update_drawing_pos(QPoint pos);
    virtual void handle_new_pos(QPoint pos) = 0;
    virtual cv::Mat draw(cv::Mat &frame) = 0;
protected:
    int LINE_THICKNESS = 2;

    cv::Scalar qcolor2scalar(QColor color);
    void setup_paint_tool(QPainter &painter);

    QColor colour;

    QPoint draw_start;
    QPoint draw_end;
};

#endif // SHAPES_H
