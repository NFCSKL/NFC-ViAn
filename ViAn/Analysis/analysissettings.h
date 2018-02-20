#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H
#include "Project/Analysis/analysis.h"
#include <opencv2/core.hpp>
/**
 * @brief The AnalysisSettings class
 * This class is uesd to configure
 * settings for analysismethod.
 */
class AnalysisSettings {

public:
    ANALYSIS_TYPE type;
    cv::Rect bounding_box;
    std::pair<int, int> interval;
    bool default_settings;
    bool use_bounding_box = false;
    bool use_interval = false;
    AnalysisSettings(ANALYSIS_TYPE type, std::pair<int, int> interval, cv::Rect bounding_box);
    AnalysisSettings(ANALYSIS_TYPE type);
    virtual ~AnalysisSettings();
    cv::Rect getBounding_box() const;
    void setBounding_box(const cv::Rect &value);
    ANALYSIS_TYPE getType() const;
    std::pair<int, int> get_interval() const;
    void setInterval(const std::pair<int, int> &value);
};

#endif // ANALYSISSETTINGS_H
