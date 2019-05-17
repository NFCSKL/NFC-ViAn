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
    m_upper_left = upper_left;
    m_lower_right = down_right;
}

/**
 * @brief DetectionBox::DetectionBox
 * @param upper_left
 * @param height
 * @param width
 */
DetectionBox::DetectionBox(std::pair<int, int> upper_left, int height, int width){
    m_upper_left = upper_left;
    m_lower_right = std::make_pair(upper_left.second + width, upper_left.second + height);
}

/**
 * @brief DetectionBox::DetectionBox
 * Creates a detectionbox from a cv::Rect.
 * @param rect
 */
DetectionBox::DetectionBox(cv::Rect rect) {
    m_upper_left = std::make_pair(rect.x, rect.y);
    m_lower_right = std::make_pair(rect.x + rect.width, rect.y + rect.height);
}

void DetectionBox::set_class_name(std::string name) {
    class_name = name;
}

void DetectionBox::set_confidence(float conf) {
    confidence = conf;
}

std::string DetectionBox::get_class_name() {
    return class_name;
}

float DetectionBox::get_confidence() {
    return confidence;
}

/**
 * @brief DetectionBox::read
 * Reads DetectionBox from json format.
 * @param json
 */
void DetectionBox::read(const QJsonObject &json){
    m_upper_left.first = json["UL_X"].toInt();
    m_upper_left.second = json["UL_Y"].toInt();

    m_lower_right.first = json["LR_X"].toInt();
    m_lower_right.second = json["LR_Y"].toInt();
}

/**
 * @brief DetectionBox::write
 * Writes DetectionBox to json format.
 * @param json
 */
void DetectionBox::write(QJsonObject &json){
    json["UL_X"] = m_upper_left.first;
    json["UL_Y"] = m_upper_left.second;

    json["LR_X"] = m_lower_right.first;
    json["LR_Y"] = m_lower_right.second;
}

/**
 * @brief DetectionBox::get_rect
 * Returns the rectangle specified by the DetectionBox.
 * @return
 */
cv::Rect DetectionBox::get_rect() {
    return cv::Rect(m_upper_left.first, m_upper_left.second, m_lower_right.first - m_upper_left.first,
                      m_lower_right.second - m_upper_left.second);
}

/**
 * @brief DetectionBox::scale_coordinates
 * Scales the coordinates in the DetectionBox according to the scale ratio parameter.
 * @param scale_ratio
 */
void DetectionBox::scale_coordinates(float scale_ratio) {
    m_upper_left.first *= scale_ratio;
    m_upper_left.second *= scale_ratio;
    m_lower_right.first *= scale_ratio;
    m_lower_right.second *= scale_ratio;
}
