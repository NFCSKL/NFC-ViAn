#include "motiondetection.h"

#include "analysissettings.h"
#include "Project/Analysis/detectionbox.h"
#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief MotionDetection::MotionDetection
 * @param source_file
 */
MotionDetection::MotionDetection(const std::string& source_file, const std::string& save_file, AnalysisSettings* settings) : AnalysisMethod(source_file, save_file, settings) {
    m_analysis.type = MOTION_DETECTION; 
}

MotionDetection::~MotionDetection() {
    background_subtractor.release();
    dilation_kernel.release();
}

/**
 * @brief MotionDetection::setup_analysis
 * Initial setup of the analysis
 */
void MotionDetection::setup_analysis(){   
    background_subtractor = cv::createBackgroundSubtractorMOG2(get_setting("BACKGROUND_HISTORY"),
                                                               get_setting("MOG2_THRESHOLD"),
                                                               get_setting("DETECT_SHADOWS"));
    dilation_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(get_setting("OPEN_DEGREE"),
                                                                         get_setting("OPEN_DEGREE")));
}


/**
 * @brief MotionDetection::analyse_frame
 * This method detects motion in a frame, partly by comparing it to the previous frame
 * and partly by using a background subtraction algorithm that detects things that are
 * not part of the background. Rectangles that mark the detected areas are saved for use
 * during video playback.
 */
std::vector<DetectionBox> MotionDetection::analyse_frame(){
    std::vector<DetectionBox> OOIs;
    std::vector<std::vector<cv::Point> > contours;
    // Updates background model
    background_subtractor->apply(analysis_frame, temp,-1);
    cv::threshold(temp, foreground_mask, DETECTION_THRESHOLD, GRAYSCALE_WHITE, cv::THRESH_BINARY);
    cv::morphologyEx(foreground_mask, result, cv::MORPH_OPEN, dilation_kernel);

    // Creates OOIs from the detected countours.
    cv::findContours(result, contours, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    for (std::vector<cv::Point> contour : contours) {
        if (cv::contourArea(contour) > get_setting("SMALLEST_OBJECT_SIZE")) {            
            cv::Rect rect = cv::boundingRect(contour);
            if(analysis_settings->use_bounding_box) {
                cv::Rect slice_rect = analysis_settings->bounding_box;
                cv::Rect rect_to_original (rect.tl()+slice_rect.tl(), slice_rect.tl()+rect.br());
                rect = rect_to_original;
            }
            if(scaling_needed) rect = Utility::scale_rect(rect, scaling_ratio, cv::Point(0,0));
            OOIs.push_back(DetectionBox(rect));
        }
    }    
    return OOIs;
}
