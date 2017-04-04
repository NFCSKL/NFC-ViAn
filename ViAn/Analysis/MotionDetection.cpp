#include "MotionDetection.h"
#include <qdebug.h>



MotionDetection::MotionDetection(std::string source_file){
    capture.open("Pumparna.avi");
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

    capture.set(CV_CAP_PROP_POS_FRAMES, 250);
    cv::namedWindow("First frame");
    cv::namedWindow("Current frame");
    cv::namedWindow("Threshold");

    pMOG2 = cv::createBackgroundSubtractorMOG2(500,50,false);
}

/**
 * @brief MotionDetection::do_analysis
 * Motion detection specific code
 * This function is called from the interface thread loop
 */
void MotionDetection::do_analysis(){
    std::vector<std::vector<cv::Point> > contours;
    // Grayscale and blur
    cv::Mat shown_frame = frame.clone();
    cv::GaussianBlur(frame, frame, blur_size, 0);
    pMOG2->apply(frame, fgMaskMOG2,-1);
    pMOG2->getBackgroundImage(background);
    cv::imshow("Threshold", background);
    cv::Mat kernel_ero = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4,4));
    cv::dilate(fgMaskMOG2, fgMaskMOG2, kernel_ero);
    cv::GaussianBlur(frame, frame, blur_size, 0);
    cv::GaussianBlur(fgMaskMOG2, fgMaskMOG2, blur_size, 0);
    cv::threshold(fgMaskMOG2, fgMaskMOG2, 25, 255, cv::THRESH_BINARY);
    cv::imshow("Current frame",fgMaskMOG2);
    cv::findContours(fgMaskMOG2.clone(), contours, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    cv::Scalar color(0,0,255);
    for (std::vector<cv::Point> contour : contours) {
        if (cv::contourArea(contour) > 500) {
            cv::Rect rect = cv::boundingRect(contour);
            cv::rectangle(shown_frame, rect, color, 2);
        }
    }
    cv::imshow("First frame", shown_frame);
    /*cv::cvtColor(frame, gray, CV_RGB2GRAY);
    cv::GaussianBlur(gray, gray, blur_size, 0);

    if (first_frame.empty()) {
        // Save first frame to use it to compare for motion
        first_frame = gray.clone();
    }



    cv::absdiff(first_frame, gray, frame_delta);
    cv::threshold(frame_delta, frame_thresh, 25, 255, cv::THRESH_BINARY);


    cv::findContours(frame_thresh.clone(), contours, cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    cv::Scalar color(0,0,255);
    cv::Mat tmp;
    for (std::vector<cv::Point> contour : contours) {
        if (cv::contourArea(contour) > 200) {
            cv::Rect rect = cv::boundingRect(contour);
            qDebug() << rect.x << "::" << rect.y << rect.width << "::" << rect.height;
            cv::cvtColor(frame_thresh, tmp, CV_GRAY2RGB);
            cv::rectangle(frame, rect, color, 2);
            cv::rectangle(tmp, rect, color, 2);
        }
    }
    if (!tmp.empty()) {
        cv::imshow("Threshold", tmp);
    }
    cv::imshow("Current frame", frame);*/


}
