#include "detectionbox.h"

/**
 * @brief DetectionBox::DetectionBox
 */
DetectionBox::DetectionBox(){}

DetectionBox::~DetectionBox(){}

/**
 * @brief DetectionBox::DetectionBox
 * @param upper_left
 * @param down_right
 */
DetectionBox::DetectionBox(std::pair<int, int> upper_left, std::pair<int, int> down_right){
    this->upper_left = upper_left;
    this->lower_right = down_right;
}

/**
 * @brief DetectionBox::DetectionBox
 * @param upper_left
 * @param height
 * @param width
 */
DetectionBox::DetectionBox(std::pair<int, int> upper_left, int height, int width){
    this->upper_left = upper_left;
    this->lower_right = std::make_pair(upper_left.second + width, upper_left.second + height);
}

/**
 * @brief DetectionBox::DetectionBox
 * Creates a detectionbox from a cv::Rect.
 * @param rect
 */
DetectionBox::DetectionBox(cv::Rect rect) {
    upper_left = std::make_pair(rect.x, rect.y);
    lower_right = std::make_pair(rect.x + rect.width, rect.y + rect.height);
}

/**
 * @brief DetectionBox::read
 * Reads DetectionBox from json format.
 * @param json
 */
void DetectionBox::read(const QJsonObject &json){
    this->upper_left.first = json["UL_X"].toInt();
    this->upper_left.second = json["UL_Y"].toInt();

    this->lower_right.first = json["LR_X"].toInt();
    this->lower_right.second = json["LR_Y"].toInt();
}

/**
 * @brief DetectionBox::write
 * Writes DetectionBox to json format.
 * @param json
 */
void DetectionBox::write(QJsonObject &json){
    json["UL_X"] = this->upper_left.first;
    json["UL_Y"] = this->upper_left.second;

    json["LR_X"] = this->lower_right.first;
    json["LR_Y"] = this->lower_right.second;
}

/**
 * @brief DetectionBox::get_rect
 * Returns the rectangle specified by the DetectionBox.
 * @return
 */
cv::Rect DetectionBox::get_rect() {
    return cv::Rect(upper_left.first, upper_left.second, lower_right.first - upper_left.first,
                      lower_right.second - upper_left.second);
}

/**
 * @brief DetectionBox::scale_coordinates
 * Scales the coordinates in the DetectionBox according to the scale ratio parameter.
 * @param scale_ratio
 */
void DetectionBox::scale_coordinates(float scale_ratio) {
    this->upper_left.first *= scale_ratio;
    this->upper_left.second *= scale_ratio;
    this->lower_right.first *= scale_ratio;
    this->lower_right.second *= scale_ratio;
}
