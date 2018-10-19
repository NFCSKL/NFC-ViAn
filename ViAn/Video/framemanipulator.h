#ifndef FRAMEMANIPULATOR_H
#define FRAMEMANIPULATOR_H

#include <opencv2/core/core.hpp>

class FrameManipulator
{
    double alpha = 1;   // Contrast, value in range CONTRAST_MIN to CONTRAST_MAX.
    int beta = 0;       // Brightness, value in range BRIGHTNESS_MIN to BRIGHTNESS_MAX.
public:
    FrameManipulator();
    void set_brightness(int value);
    void set_contrast(double value);
    int get_brightness();
    double get_contrast();
    void contrast_frame(cv::Mat &src);
    void apply(cv::Mat& src);
    void reset();

    // Constants for the limits and the precision of contrast and brightness values.
    const double CONTRAST_MIN = 0.5, CONTRAST_MAX = 5, CONTRAST_DEFAULT = 1, CONTRAST_STEP = 0.01;
    const int CONTRAST_DECIMALS = 2;
    const int BRIGHTNESS_MIN = -100, BRIGHTNESS_MAX = 100, BRIGHTNESS_DEFAULT = 0, BRIGHTNESS_STEP = 1;
};

#endif // FRAMEMANIPULATOR_H
