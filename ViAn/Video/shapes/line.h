#ifndef LINE_H
#define LINE_H

#include "shape.h"

class line : public shape {
public:
    line(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos);
};

#endif // LINE_H
