#ifndef ANALYSAREA_H
#define ANALYSAREA_H

#include "opencv2/opencv.hpp"
#include "shape.h"

class AnalysArea {
public:
    AnalysArea();
    void add_point(QPoint pos);
private:
    std::vector<cv::Point> points;
};

#endif // ANALYSAREA_H
