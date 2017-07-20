#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H

#include "AnalysisMethod.h"
#include "AnalysisController.h"
#include <string>

/**
 * @brief The MotionDetection class
 * This class is used to detect motion in videos. This
 * is done using background subtraction with OpenCV.
 */
class MotionDetection : public AnalysisMethod {
public:
    MotionDetection(std::string source_file);
    ~MotionDetection();

private:
    cv::Mat foreground_mask, background, blurred_frame, diff_frame, result, prev_frame, dilation_kernel;
    cv::Ptr<cv::BackgroundSubtractor> background_subtractor;
    const cv::Size BLUR_SIZE = cv::Size(31,31);

    void setup_analysis() override;
    std::vector<DetectionBox> analyse_frame() override;

    const int DILATION_DEGREE = 16;
    const int SMALLEST_OBJECT_SIZE = 500;
    const int DETECTION_THRESHOLD = 25;
    const int GRAYSCALE_WHITE = 255;
    const int BACKGROUND_HISTORY = 500;
    const int MOG2_THRESHOLD = 50;
    const bool DETECT_SHADOWS = false;


signals:
    void save_analysis(Analysis analysis);
};

#endif // MOTIONDETECTION_H
