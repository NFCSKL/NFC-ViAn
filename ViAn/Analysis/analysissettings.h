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
    AnalysisInterval interval;
    QPoint box_start, box_end;
    bool default_settings;
    bool use_bounding_box = false;
    bool use_interval = false;
    AnalysisSettings(ANALYSIS_TYPE type, AnalysisInterval interval, cv::Rect bounding_box);
    AnalysisSettings(ANALYSIS_TYPE type);
    virtual ~AnalysisSettings();
    cv::Rect getBounding_box() const;
    void setBounding_box(const cv::Rect &value);
    void set_bounding_box_points(const QPoint &start, const QPoint &end);
    ANALYSIS_TYPE getType() const;
    AnalysisInterval getInterval() const;
    QPoint get_box_start() const;
    QPoint get_box_end() const;
    void setInterval(const AnalysisInterval &value);
};

#endif // ANALYSISSETTINGS_H
