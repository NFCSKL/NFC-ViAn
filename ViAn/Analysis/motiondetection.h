#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H
#include "Project/Analysis/detectionbox.h"
#include "analysismethod.h"
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
    cv::Mat foreground_mask, result, dilation_kernel, temp;

    /**
     * @brief background_subtractor
     * OBS! TODO: This parameter seems to be leaking memory during analysis
     * doesn't seem to be destroyed properly
     */
    cv::Ptr<cv::BackgroundSubtractor> background_subtractor;

    void init_settings() override;
    void setup_analysis() override;
    std::vector<DetectionBox> analyse_frame() override;

    const int DETECTION_THRESHOLD = 25;
    const int GRAYSCALE_WHITE = 255;
    const cv::Size BLUR_SIZE = cv::Size(31,31);
};

#endif // MOTIONDETECTION_H
