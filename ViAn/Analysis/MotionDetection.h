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
    cv::Mat fg_mask;                                 // Foreground mask
    cv::Ptr<cv::BackgroundSubtractor> bg_sub;        // MOG2 Background subtractor

    void setup_analysis() override;
    void do_analysis() override;


protected:

};

#endif // MOTIONDETECTION_H
