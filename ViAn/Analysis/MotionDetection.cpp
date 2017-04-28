#include "MotionDetection.h"
#include <vector>

/**
 * @brief MotionDetection::MotionDetection
 * @param source_file
 */
MotionDetection::MotionDetection(std::string source_file) {
    m_analysis.type = MOTION_DETECTION;
    capture.open(source_file);
}

/**
 * @brief MotionDetection::setup_analysis
 * Initial setup of the analysis
 */
void MotionDetection::setup_analysis(){
    pMOG2 = cv::createBackgroundSubtractorMOG2(500,50,false);
}

/**
 * @brief MotionDetection::do_analysis
 * Motion detection specific code
 * This function is called from the interface thread loop
 */
std::vector<OOI> MotionDetection::analyse_frame(){
    std::vector<OOI> OOIs;
    std::vector<std::vector<cv::Point> > contours;

    shown_frame = frame.clone();

    cv::GaussianBlur(frame, frame, blur_size, 0);
    pMOG2->apply(frame, fgMaskMOG2,-1);
    pMOG2->getBackgroundImage(background);

    cv::Mat kernel_ero = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilation_degree,dilation_degree));
    cv::threshold(fgMaskMOG2, fgMaskMOG2, 25, 255, cv::THRESH_BINARY);
    cv::dilate(fgMaskMOG2, fgMaskMOG2, kernel_ero);

    if (!diff_prev.empty() && current_frame % sample_freq == 0) {
        cv::Mat gray_frame = shown_frame.clone();
        cv::cvtColor(gray_frame, gray_frame, CV_RGB2GRAY);
        cv::absdiff(diff_prev,gray_frame,diff_frame);
        cv::GaussianBlur(diff_frame, diff_frame, blur_size, 0);
        cv::threshold(diff_frame, diff_frame, 25, 255, cv::THRESH_BINARY);
        cv::dilate(diff_frame, diff_frame, kernel_ero);
        cv::bitwise_and(diff_frame, fgMaskMOG2, result);
    } else if (!diff_prev.empty() && !diff_frame.empty()) {
        cv::bitwise_and(diff_frame, fgMaskMOG2, result);
    } else {
        result = fgMaskMOG2.clone();
    }

    if (current_frame % sample_freq == 0) {
        diff_prev = shown_frame.clone();
        cv::cvtColor(diff_prev, diff_prev, CV_RGB2GRAY);
    }
    prev_frame = shown_frame.clone();
    cv::cvtColor(prev_frame, prev_frame, CV_RGB2GRAY);

    //This code excludes the area of the frame that has been given by exclude_frame.
    if (do_exclusion) {
        cv::bitwise_and(result, exclude_frame, result);
    }

    cv::findContours(result.clone(), contours, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    cv::Scalar color(0,0,255);
    std::vector<cv::Rect> rects;
    for (std::vector<cv::Point> contour : contours) {
        if (cv::contourArea(contour) > smallest_object_size) {
            cv::Rect rect = cv::boundingRect(contour);
            rects.push_back(rect);
            OOIs.push_back(OOI(rect));
        }
    }

    return OOIs;
}
