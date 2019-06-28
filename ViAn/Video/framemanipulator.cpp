#include "framemanipulator.h"
#include "constants.h"
#include <QDebug>
//#include <iostream>

FrameManipulator::FrameManipulator() {
    // TODO Move alpha beta gamma to constants?
    reset();
}

/**
 * @brief FrameManipulator::set_brightness
 * Sets the brightness value
 * @param value
 */
void FrameManipulator::set_brightness(int value){
    beta = std::min(Constants::BRIGHTNESS_MAX, std::max(Constants::BRIGHTNESS_MIN, value));
}

/**
 * @brief FrameManipulator::set_contrast
 * Sets the contrast value
 * @param value
 */
void FrameManipulator::set_contrast(double value) {
    alpha = std::min(Constants::CONTRAST_MAX, std::max(Constants::CONTRAST_MIN, value));
}

/**
 * @brief FrameManipulator::set_gamma
 * Sets the gamma value
 * @param value
 */
void FrameManipulator::set_gamma(double value) {
    gamma = std::min(Constants::GAMMA_MAX, std::max(Constants::GAMMA_MIN, value));
}

int FrameManipulator::get_brightness() {
    return beta;
}

double FrameManipulator::get_contrast() {
    return alpha;
}

double FrameManipulator::get_gamma() {
    return gamma;
}


/**
 * @brief FrameManipulator::apply
 * Applies the contrast and brightness values to the src frame
 * @param src
 */
void FrameManipulator::apply(cv::Mat &src) {
    // Do the operation modified_frame = alpha * frame + beta
    src.convertTo(src, -1, alpha, beta);
    src = correct_gamma(src, gamma);
}

cv::Mat FrameManipulator::correct_gamma(cv::Mat& img, double gamma) {
    double inverse_gamma = 1.0 / gamma;

    cv::Mat lut_matrix(1, 256, CV_8UC1);
    uchar * ptr = lut_matrix.ptr();
    for (int i = 0; i < 256; i++)
        ptr[i] = (int)(pow((double)i / 255.0, inverse_gamma) * 255.0);

    cv::Mat result;
    cv::LUT(img, lut_matrix, result);

    return result;
}

/**
 * @brief FrameManipulator::reset
 * Reset the alpha and beta values to default
 */
void FrameManipulator::reset() {
    alpha = Constants::CONTRAST_DEFAULT;
    beta = Constants::BRIGHTNESS_DEFAULT;
    gamma = Constants::GAMMA_DEFAULT;
}
