#ifndef ARROW_H
#define ARROW_H

#include "shape.h"

class arrow : public shape {
public:
    arrow(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
};

#endif // ARROW_H
