#ifndef UTILITY_H
#define UTILITY_H

#include "opencv2/core/core.hpp"

#include <QRect>
#include <QSize>
#include <QDir>
#include <QCryptographicHash>

namespace Utility{
     std::pair<double, double> size_ratio(QSize s1, QSize s2);
     double min_size_ratio(QSize s1, QSize s2);
     int number_of_digits(int n);
     std::string zfill(std::string number, int length);
     std::string zfill(const int& number, int length);
     std::string name_from_path(const std::string full_path);
     std::string add_serial_number(std::string name, std::string file_end);
     std::string remove_serial_number(std::string file);

     cv::Rect scale_rect(cv::Rect rect, double scale_factor, cv::Point anchor);
     QRect scale_rect(QRect rect, double scale_factor, QPoint anchor);

     QPoint from_cvpoint(cv::Point point);
     cv::Point from_qpoint(QPoint point);
     cv::Rect from_qrect(QRect rect);
     QRect from_cvrect(cv::Rect rect);
     QByteArray checksum(const QString& file,
                         QCryptographicHash::Algorithm hash_algorithm=QCryptographicHash::Algorithm::Sha256);
     bool remove_checksum_files(const QString& parent_folder, const QStringList& hashes);

     QPoint rotate(QPoint pos, int rotation, int width, int height);
}

#endif // UTILITY_H

