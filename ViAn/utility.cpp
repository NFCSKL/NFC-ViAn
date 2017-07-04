#include "utility.h"

Utility::Utility(){
}

/**
 * @brief Utility::size_ratio
 * calculates and returns the height and width ratios between the two QSize objects
 * @param s1
 * @param s2
 * @return
 */
std::pair<double, double> Utility::size_ratio(QSize s1, QSize s2) {
    return std::make_pair(double(s1.width()) / s2.width(), double(s1.height()) / s2.height());
}

double Utility::min_size_ratio(QSize s1, QSize s2) {
    std::pair<double, double> ratios = size_ratio(s1, s2);
    return std::min(ratios.first, ratios.second);
}
