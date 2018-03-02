#include "analysissettings.h"

AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type,
                                   std::pair<int, int> interval,
                                   cv::Rect bounding_box)
    : type(type)
{
    setInterval(interval);
    setBounding_box(bounding_box);
}

AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type) : type(type) {
    default_settings = true;
}


AnalysisSettings::~AnalysisSettings() {
}


cv::Rect AnalysisSettings::getBounding_box() const {
    return bounding_box;
}

void AnalysisSettings::setBounding_box(const cv::Rect &value) {
    bounding_box = value;
    use_bounding_box = true;
}

ANALYSIS_TYPE AnalysisSettings::getType() const {
    return type;
}

std::pair<int, int> AnalysisSettings::get_interval() const {
    return interval;
}

void AnalysisSettings::setInterval(const std::pair<int, int> &value) {
    interval = value;
    use_interval = true;
}
