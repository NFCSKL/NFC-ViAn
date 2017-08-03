#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H
#include "Project/Analysis/detectionbox.h"
#include "analysismethod.h"
#include "analysiscontroller.h"
#include <string>

/**
 * @brief The MotionDetection class
 * This class is used to detect motion in videos. This
 * is done using background subtraction with OpenCV.
 */
class MotionDetection : public AnalysisMethod {
public:

    MotionDetection(std::string source_file, std::string save_file);
    ~MotionDetection();

private:
    cv::Mat foreground_mask, background, blurred_frame, diff_frame, result, prev_frame, dilation_kernel;
    cv::Ptr<cv::BackgroundSubtractor> background_subtractor;

    void init_settings() override;
    void setup_analysis() override;
    std::vector<DetectionBox> analyse_frame() override;

    const int DETECTION_THRESHOLD = 25;
    const int GRAYSCALE_WHITE = 255;
    const cv::Size BLUR_SIZE = cv::Size(31,31);

    const int OPEN_DEGREE = 16;
    const int SMALLEST_OBJECT_SIZE = 500;
    const int BACKGROUND_HISTORY = 500;
    const int MOG2_THRESHOLD = 50;
    const bool IGNORE_SHADOWS = false;

signals:
    void save_analysis(Analysis analysis);
};

#endif // MOTIONDETECTION_H
