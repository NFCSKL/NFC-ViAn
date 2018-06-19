#ifndef ANALYSAREA_H
#define ANALYSAREA_H

#include "opencv2/opencv.hpp"
#include "shapes.h"

class AnalysArea {
public:
    AnalysArea();
    ~AnalysArea();
    cv::Mat draw(cv::Mat &frame);
    void add_point(QPoint pos);
    void invert_area();
    bool is_including_area();
    std::vector<cv::Point>* get_polygon();
    void undo();
    void clear();
private:
    std::vector<cv::Point>* points = new std::vector<cv::Point>();
    // Bool indicating if an area for analysis or an area for
    // not running analysis is being choosen.
    bool include_area_to_analyse = true;
};

#endif // ANALYSAREA_H
