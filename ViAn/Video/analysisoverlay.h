#ifndef ANALYSISOVERLAY_H
#define ANALYSISOVERLAY_H

#include "opencv2/opencv.hpp"

class AnalysisOverlay {
public:
    AnalysisOverlay();
    cv::Mat draw_overlay(cv::Mat &frame, int frame_nr);
    void add_area(int frame_nr, cv::Point start, cv::Point end);
    bool is_showing_overlay();
    void toggle_showing();
private:
    // Map of vectors with start and end coordinates (as a pair of points)
    std::map<int, std::vector< std::pair<cv::Point, cv::Point> > > detections;

    bool showing_overlay = true;
};

#endif // ANALYSISOVERLAY_H
