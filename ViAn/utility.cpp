#include "utility.h"
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

int Utility::number_of_digits(int n) {
    int number_of_digits = 0;
    do {
         ++number_of_digits;
         n /= 10;
    } while (n);
    return number_of_digits;
}

/**
 * @brief Utility::zfill
 * Fills the given number string with zeros
 * @param number    :    number to be zero padded
 * @param length    :   specifies how long the return string should be
 * @return
 */
std::string Utility::zfill(std::string number, int length){
    length -= number.length();
    if (length)
        return std::string(length, '0').append(number);
    else
        return number;
}

std::string Utility::name_from_path(const std::string full_path)
{
    std::string res = full_path.substr(full_path.find_last_of("/")+1, full_path.size());
    return res;
}

cv::Rect Utility::scale_rect(cv::Rect rect, double scale_factor, cv::Point anchor)
{
    cv::Point start(anchor.x + rect.tl().x/scale_factor, anchor.y = rect.tl().y/scale_factor);
    double height_diff = rect.br().y - rect.tl().y;
    double width_diff = rect.br().x - rect.tl().x;
    cv::Point end (anchor.x + rect.tl().x + width_diff/scale_factor, anchor.y + rect.tl().y + height_diff/scale_factor);
    cv::Rect res = cv::Rect(start, end);
    return res;
}

QRect Utility::scale_rect(QRect rect, double scale_factor, QPoint anchor){
    return from_cvrect(scale_rect(from_qrect(rect), scale_factor, from_qpoint(anchor)));
}

QPoint Utility::from_cvpoint(cv::Point point)
{
    return QPoint(point.x,point.y);
}

cv::Point Utility::from_qpoint(QPoint point)
{
    return cv::Point(point.x(),point.y());
}

cv::Rect Utility::from_qrect(QRect rect)
{
    return cv::Rect(from_qpoint(rect.topLeft()), from_qpoint(rect.bottomRight()));
}

QRect Utility::from_cvrect(cv::Rect rect)
{
    return QRect(from_cvpoint(rect.tl()), from_cvpoint(rect.br()));
}
