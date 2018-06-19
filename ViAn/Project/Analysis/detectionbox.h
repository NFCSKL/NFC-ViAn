#ifndef DETECTIONBOX_H
#define DETECTIONBOX_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"

class DetectionBox : Saveable{
    int frame;
    std::pair<int, int> upper_left;
    std::pair<int, int> lower_right;
public:
    DetectionBox();
    DetectionBox(std::pair<int,int> upper_left, std::pair<int,int> lower_right);
    DetectionBox(std::pair<int,int> upper_left, int height, int width);
    DetectionBox(cv::Rect rect);
    ~DetectionBox() override;

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    cv::Rect get_rect();
    void scale_coordinates(float scale_ratio);
};
#endif // DETECTIONBOX_H
