#ifndef ANALYSISOVERLAY_H
#define ANALYSISOVERLAY_H

#include "opencv2/opencv.hpp"

class AnalysisOverlay {
public:
    AnalysisOverlay();
    cv::Mat draw_overlay(cv::Mat &frame, int frame_nr);
private:
    //std::map<int, std::vector<XX>> detections;
};

#endif // ANALYSISOVERLAY_H
