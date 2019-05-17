#ifndef DETECTIONBOX_H
#define DETECTIONBOX_H

#include "Filehandler/saveable.h"

#include "opencv2/core/core.hpp"

#include <QJsonObject>

class DetectionBox : Saveable {
    std::pair<int, int> m_upper_left;
    std::pair<int, int> m_lower_right;

    std::string class_name;
    float confidence;
public:
    DetectionBox();
    DetectionBox(std::pair<int,int> upper_left, std::pair<int,int> lower_right);
    DetectionBox(std::pair<int,int> upper_left, int height, int width);
    DetectionBox(cv::Rect rect);
    ~DetectionBox() override;

    void set_class_name(std::string name);
    void set_confidence(float conf);

    std::string get_class_name();
    float get_confidence();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
    cv::Rect get_rect();
    void scale_coordinates(float scale_ratio);
};
#endif // DETECTIONBOX_H
