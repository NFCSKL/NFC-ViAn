#ifndef OOI_H
#define OOI_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"

class OOI : Saveable{
    int frame;
    std::string file_path;
    std::pair<int, int> upper_left;
    std::pair<int, int> lower_right;
public:
    OOI();
    OOI(std::pair<int,int> upper_left, std::pair<int,int> lower_right);
    OOI(std::pair<int,int> upper_left, int height, int width);
    OOI(cv::Rect rect);

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    cv::Rect get_rect();
    void scale_coordinates(float scale_ratio);
};
#endif // OOI_H
