#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H

#include "AnalysisMethod.h"
#include <string>

/**
 * @brief The MotionDetection class
 * This class is used to detect motion in videos. This
 * is done using background subtraction with OpenCV.
 */
class MotionDetection : public AnalysisMethod {
public:
    MotionDetection(std::string source_file);

private:
/*
    cv::Mat first_frame;                             // First frame in the video
    cv::Mat gray;                                    // Grayscale of current
    cv::Mat frame_delta, frame_thresh, frame_dilate;
    cv::Size blur_size = cv::Size(31,31);*/

    cv::Mat fgMaskMOG2, background, prev_frame, shown_frame, diff_frame, result, diff_prev; //fg mask fg mask generated by MOG2 method
    cv::Ptr<cv::BackgroundSubtractor> pMOG2;
    cv::Size blur_size = cv::Size(31,31);


    void setup_analysis() override;
    std::vector<OOI> analyse_frame() override;
    void merge_bounding_rects(std::vector<cv::Rect> &rects);

    unsigned int dilation_degree = 16;
    unsigned int smallest_object_size = 500;
protected:

signals:
    void save_analysis(Analysis analysis);

};

#endif // MOTIONDETECTION_H
