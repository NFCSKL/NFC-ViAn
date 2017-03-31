#include "MotionDetection.h"
#include <iostream>


MotionDetection::MotionDetection(std::string source_file){
    capture.open(source_file);
    if (!capture.isOpened()) {
        // TODO Take care of this
        std::cout << "Failed to load " << source_file << std::endl;
    }
}

/**
 * @brief MotionDetection::setup_analysis
 * Initial setup of the analysis
 */
void MotionDetection::setup_analysis(){
    bg_sub = cv::createBackgroundSubtractorMOG2();
    //create GUI windows
    cv::namedWindow("Frame");
    cv::namedWindow("FG Mask MOG 2");
}

/**
 * @brief MotionDetection::do_analysis
 * Motion detection specific code
 * This function is called from the interface thread loop
 */
void MotionDetection::do_analysis(){
    bg_sub->apply(frame, fg_mask);  // Updated the background model
    cv::imshow("Frame", frame);
    cv::imshow("FG Mask MOG 2", fg_mask);
}
