#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class circle : public shape {
public:
    circle(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
};

#endif // CIRCLE_H
