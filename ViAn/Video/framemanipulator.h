#ifndef FRAMEMANIPULATOR_H
#define FRAMEMANIPULATOR_H

#include <opencv2/core/core.hpp>

class FrameManipulator
{
    double alpha;   // Contrast, value in range CONTRAST_MIN to CONTRAST_MAX.
    int beta;       // Brightness, value in range BRIGHTNESS_MIN to BRIGHTNESS_MAX.
    double gamma;   // Gamma, value in range GAMMA_MIN to GAMMA_MAX.
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
};

#endif // FRAMEMANIPULATOR_H
