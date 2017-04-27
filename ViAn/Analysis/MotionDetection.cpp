#include "MotionDetection.h"
#include <qdebug.h>
#include <vector>



MotionDetection::MotionDetection(std::string source_file) {
    m_analysis.type = MOTION_DETECTION;
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
    // Grayscale and blur

    shown_frame = frame.clone();

    cv::GaussianBlur(frame, frame, blur_size, 0);
    pMOG2->apply(frame, fgMaskMOG2,-1);

    pMOG2->getBackgroundImage(background);


    cv::Mat kernel_ero = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilation_degree,dilation_degree));
    //cv::GaussianBlur(fgMaskMOG2, fgMaskMOG2, blur_size, 0);
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

    if (!rects.empty()) {
        /*merge_bounding_rects(rects);
        for(cv::Rect rect : rects) {
            cv::rectangle(shown_frame, rect, color, 2);
        }*/
    }

    return OOIs;
}

void MotionDetection::merge_bounding_rects(std::vector<cv::Rect> &rects) {
    std::vector<cv::Rect> result;
    std::vector<int> removed_rects;

    while (rects.size() > 1) {
        cv::Rect rect1 = rects.back();
        rects.pop_back();
        for(int i = 0; i < rects.size(); ++i) {
            cv::Rect rect2 = rects[i];
            if ((rect1 & rect2).area() > 0) {
                rect1 = (rect1 | rect2);
                removed_rects.push_back(i);
            }
        }
        std::reverse(removed_rects.begin(),removed_rects.end());
        for(int i : removed_rects) {
            rects.erase(rects.begin() + i);
        }
        result.push_back(rect1);
    }
    result.push_back(rects.front());
    rects = result;
}
