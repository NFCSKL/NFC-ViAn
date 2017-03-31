#ifndef PEN_H
#define PEN_H

#include "shape.h"

class Pen : public Shape {
public:
    Pen(QColor col, QPoint pos);
    cv::Mat draw(cv::Mat &frame) override;
    void handle_new_pos(QPoint pos) override;
private:
    std::vector<std::pair<cv::Point, cv::Point>> lines;
};

#endif // PEN_H
