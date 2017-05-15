#ifndef ANALYSISOVERLAY_H
#define ANALYSISOVERLAY_H

#include "opencv2/opencv.hpp"

class AnalysisOverlay {
public:
    AnalysisOverlay();
    cv::Mat draw_overlay(cv::Mat &frame, int frame_nr);
    void add_area(int frame_nr, cv::Rect rect);
    bool is_showing_overlay();
    void toggle_showing();
private:
    // Colour and thickness for the graphical representation of the detected areas.
    const cv::Scalar COLOUR = cv::Scalar(255, 0, 0);
    const int THICKNESS = 3;

    // The detected areas mapped to the frame they're on.
    std::map<int, std::vector<cv::Rect>> detections;

    bool showing_overlay = true;
};

#endif // ANALYSISOVERLAY_H
