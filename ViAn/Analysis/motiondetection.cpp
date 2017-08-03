#include "motiondetection.h"
#include <vector>

/**
 * @brief MotionDetection::MotionDetection
 * @param source_file
 */
MotionDetection::MotionDetection(std::string source_file, std::string save_file) : AnalysisMethod(source_file, save_file)
{
    m_analysis.type = MOTION_DETECTION;
    init_settings();    
}

MotionDetection::~MotionDetection() {

}

void MotionDetection::init_settings()
{
    add_setting("OPEN_DEGREE", 16, "Noise filtering, higher=> less noise");
    add_setting("SMALLEST_OBJECT_SIZE", 500, "Smallest detected object");
    add_setting("BACKGROUND_HISTORY", 500, "Number of frames in background model");
    add_setting("MOG2_THRESHOLD", 50, "MOG2");
    add_setting("IGNORE_SHADOWS", 0, "Ignore reflections");
}


/**
 * @brief MotionDetection::setup_analysis
 * Initial setup of the analysis
 */
void MotionDetection::setup_analysis(){    
    background_subtractor = cv::createBackgroundSubtractorMOG2(get_setting("BACKGROUND_HISTORY"),
                                                               get_setting("MOG2_THRESHOLD"),
                                                               get_setting("IGNORE_SHADOWS"));
    dilation_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(get_setting("OPEN_DEGREE"),
                                                                         get_setting("OPEN_DEGREE")));
}


/**
 * @brief MotionDetection::do_analysis
 * This method detects motion in a frame, partly by comparing it to the previous frame
 * and partly by using a background subtraction algorithm that detects things that are
 * not part of the background. Rectangles that mark the detected areas are saved for use
 * during video playback.
 */
std::vector<DetectionBox> MotionDetection::analyse_frame(){
    std::vector<DetectionBox> OOIs;
    std::vector<std::vector<cv::Point> > contours;
    // Updates background model
    background_subtractor->apply(analysis_frame, foreground_mask,-1);
    cv::threshold(foreground_mask, foreground_mask, DETECTION_THRESHOLD, GRAYSCALE_WHITE, cv::THRESH_BINARY);
    cv::morphologyEx(foreground_mask, result, cv::MORPH_OPEN, dilation_kernel);

    //This code excludes the area of the frame that has been given by exclude_frame.
    if (do_exclusion) {
        cv::bitwise_and(result, exclude_frame, result);
    }
    // Creates OOIs from the detected countours.
    cv::findContours(result, contours, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    for (std::vector<cv::Point> contour : contours) {
        if (cv::contourArea(contour) > get_setting("SMALLEST_OBJECT_SIZE")) {
            cv::Rect rect = cv::boundingRect(contour);
            if(use_bounding_box){
                cv::Rect slice_rect = bounding_box;
                cv::Rect rect_to_original (rect.tl()+slice_rect.tl(), slice_rect.tl() + rect.br());
                rect = rect_to_original;

            }
            OOIs.push_back(DetectionBox(rect));
        }
    }
    analysis_frame.release();
    return OOIs;
}
