#ifndef FRAMEMANIPULATOR_H
#define FRAMEMANIPULATOR_H

#include <opencv2/core/core.hpp>

class FrameManipulator {
    double alpha = 1;   // Contrast, value in range CONTRAST_MIN to CONTRAST_MAX.
    int beta = 0;       // Brightness, value in range BRIGHTNESS_MIN to BRIGHTNESS_MAX.
    double gamma = 1;   // Gamma, value in range GAMMA_MIN to GAMMA_MAX.
public:
    FrameManipulator();
    void set_brightness(int value);
    void set_contrast(double value);
    void set_gamma(double value);
    int get_brightness();
    double get_contrast();
    double get_gamma();
    void contrast_frame(cv::Mat &src);
    void apply(cv::Mat& src);
    cv::Mat correct_gamma(cv::Mat& img, double gamma);
    void reset();

    // Constants for the limits and the precision of contrast and brightness values.
    const double CONTRAST_MIN = 0.5, CONTRAST_MAX = 5, CONTRAST_DEFAULT = 1, CONTRAST_STEP = 0.01;
    const int BRIGHTNESS_MIN = -100, BRIGHTNESS_MAX = 100, BRIGHTNESS_DEFAULT = 0, BRIGHTNESS_STEP = 1;
    const double GAMMA_MIN = 0.01, GAMMA_MAX = 4, GAMMA_DEFAULT = 1, GAMMA_STEP = 0.01;
};

#endif // FRAMEMANIPULATOR_H
