#include "analysissettings.h"

AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type,
                                   AnalysisInterval interval,
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

void AnalysisSettings::set_bounding_box_points(const QPoint &start, const QPoint &end) {
    box_start = start;
    box_end = end;
}

ANALYSIS_TYPE AnalysisSettings::getType() const {
    return type;
}

AnalysisInterval AnalysisSettings::getInterval() const {
    return interval;
}

QPoint AnalysisSettings::get_box_start() const {
    return box_start;
}

QPoint AnalysisSettings::get_box_end() const {
    return box_end;
}

void AnalysisSettings::setInterval(const AnalysisInterval &value) {
    interval = value;
    use_interval = true;
}
