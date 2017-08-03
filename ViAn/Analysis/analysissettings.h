#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H
#include "Project/Analysis/analysis.h"
#include <opencv2/core.hpp>

class AnalysisSettings {

public:
    ANALYSIS_TYPE type;
    cv::Rect bounding_box;
    AnalysisInterval interval;
    const int SAMPLE_RATE = 1;
    bool default_settings;
    bool use_bounding_box = false;
    bool use_interval = false;
    AnalysisSettings(ANALYSIS_TYPE type, AnalysisInterval interval, cv::Rect bounding_box,
                     int sample_rate);
    AnalysisSettings(ANALYSIS_TYPE type);
    virtual ~AnalysisSettings();
    cv::Rect getBounding_box() const;
    void setBounding_box(const cv::Rect &value);

    ANALYSIS_TYPE getType() const;
    AnalysisInterval getInterval() const;
    void setInterval(const AnalysisInterval &value);
};

#endif // ANALYSISSETTINGS_H
