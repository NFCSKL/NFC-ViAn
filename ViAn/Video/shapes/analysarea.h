#ifndef ANALYSAREA_H
#define ANALYSAREA_H

#include "opencv2/opencv.hpp"
#include "shape.h"

class AnalysArea {
public:
    AnalysArea();
    ~AnalysArea();
    cv::Mat draw(cv::Mat &frame);
    void add_point(QPoint pos);
private:
    std::vector<cv::Point>* points = new std::vector<cv::Point>();
};

#endif // ANALYSAREA_H
