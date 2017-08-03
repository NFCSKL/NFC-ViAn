#include "analysissettings.h"

AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type,
                                   AnalysisInterval interval,
                                   cv::Rect bounding_box,
                                   int sample_rate)
    : type(type),
      SAMPLE_RATE(sample_rate)
{
    setInterval(interval);
    setBounding_box(bounding_box);
}

AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type) : type(type)
{
    default_settings = true;
}


AnalysisSettings::~AnalysisSettings()
{    
}


cv::Rect AnalysisSettings::getBounding_box() const
{
    return bounding_box;
}

void AnalysisSettings::setBounding_box(const cv::Rect &value)
{
    bounding_box = value;
    use_bounding_box = true;
}

ANALYSIS_TYPE AnalysisSettings::getType() const
{
    return type;
}

AnalysisInterval AnalysisSettings::getInterval() const
{
    return interval;
}

void AnalysisSettings::setInterval(const AnalysisInterval &value)
{
    interval = value;
    use_interval = true;
}
