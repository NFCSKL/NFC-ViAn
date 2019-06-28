#ifndef MOTIONDETECTION_H
#define MOTIONDETECTION_H

#include "analysismethod.h"

#include "opencv2/video/background_segm.hpp"
#include "opencv2/core/core.hpp"

class DetectionBox;

/**
 * @brief The MotionDetection class
 * This class is used to detect motion in videos. This
 * is done using background subtraction with OpenCV.
 */
class MotionDetection : public AnalysisMethod {
public:
    MotionDetection(const std::string& source_file, const std::string& save_file, AnalysisSettings *settings);
    ~MotionDetection() override;

private:
    cv::Mat foreground_mask, result, dilation_kernel, temp;

    /**
     * @brief background_subtractor
     * OBS! TODO: This parameter seems to be leaking memory during analysis
     * doesn't seem to be destroyed properly
     */
    cv::Ptr<cv::BackgroundSubtractor> background_subtractor;

    void setup_analysis() override;
    std::vector<DetectionBox> analyse_frame() override;

    const int DETECTION_THRESHOLD = 25;
    const int GRAYSCALE_WHITE = 255;
    const cv::Size BLUR_SIZE = cv::Size(31,31);
};

#endif // MOTIONDETECTION_H
