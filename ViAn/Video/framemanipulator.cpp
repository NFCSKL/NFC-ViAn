#include "framemanipulator.h"
#include <QDebug>
#include <iostream>

FrameManipulator::FrameManipulator() {
}

/**
 * @brief FrameManipulator::set_brightness
 * Sets the brightness value
 * @param value
 */
void FrameManipulator::set_brightness(int value){
    beta = std::min(BRIGHTNESS_MAX, std::max(BRIGHTNESS_MIN, value));
}

/**
 * @brief FrameManipulator::set_contrast
 * Sets the contrast value
 * @param value
 */
void FrameManipulator::set_contrast(double value) {
    alpha = std::min(FrameManipulator::CONTRAST_MAX, std::max(FrameManipulator::CONTRAST_MIN, value));
}


/**
 * @brief FrameManipulator::apply
 * Applies the contrast and brightness values to the src frame
 * @param src
 */
void FrameManipulator::apply(cv::Mat &src) {
    // Do the operation modified_frame = alpha * frame + beta
    src.convertTo(src, -1, alpha, beta);
}

/**
 * @brief FrameManipulator::reset
 * Reset the alpha and beta values to default
 */
void FrameManipulator::reset() {
    alpha = FrameManipulator::CONTRAST_DEFAULT;
    beta = FrameManipulator::BRIGHTNESS_DEFAULT;
}
