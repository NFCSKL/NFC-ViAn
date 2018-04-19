#ifndef UTILITY_H
#define UTILITY_H
#include <opencv2/core.hpp>
#include <QSize>
#include <QPoint>
#include <QRect>
namespace Utility{
     std::pair<double, double> size_ratio(QSize s1, QSize s2);
     double min_size_ratio(QSize s1, QSize s2);
     int number_of_digits(int n);
     std::string zfill(std::string number, int length);
     std::string name_from_path(const std::string full_path);

     cv::Rect scale_rect(cv::Rect rect, double scale_factor, cv::Point anchor);
     QRect scale_rect(QRect rect, double scale_factor, QPoint anchor);

     QPoint from_cvpoint(cv::Point point);
     cv::Point from_qpoint(QPoint point);
     cv::Rect from_qrect(QRect rect);
     QRect from_cvrect(cv::Rect rect);
}

#endif // UTILITY_H

