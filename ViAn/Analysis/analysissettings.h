#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H
#include "Project/Analysis/analysis.h"
#include <opencv2/core.hpp>

using Settings = std::map<std::string,int>;
using SettingsDescr = std::map<std::string,std::string>;

/**
 * @brief The AnalysisSettings class
 * This class is used to configure
 * settings for analysismethod.
 */
class AnalysisSettings {
    Settings m_settings;                // Custom integer settings for constants
    SettingsDescr m_descriptions;       // Descriptions for settings constants

public:
    ANALYSIS_TYPE type;
    cv::Rect bounding_box = cv::Rect(0,0,0,0);
    std::pair<int, int> interval = std::make_pair(-1,-1);
    bool use_bounding_box = false;
    bool use_interval = false;
    bool quick_analysis = false;

    AnalysisSettings(ANALYSIS_TYPE type, std::pair<int, int> interval, cv::Rect bounding_box);
    AnalysisSettings(ANALYSIS_TYPE type);
    AnalysisSettings();
    AnalysisSettings(AnalysisSettings *as);
    virtual ~AnalysisSettings();

    void reset_settings();
    cv::Rect getBounding_box() const;
    void setBounding_box(const cv::Rect &value);
    ANALYSIS_TYPE getType() const;
    std::pair<int, int> get_interval() const;
    void setInterval(const std::pair<int, int> &value);

    std::vector<std::string> get_var_names();
    void set_setting(const std::string &var, int value);
    void set_full_settings(Settings new_settings);
    Settings get_full_settings();
    int get_setting(const std::string& var);
    std::string get_descr(const std::string& var_name);
    std::string get_type_string();

protected:
    void add_setting(const std::string& var, int value_default, const std::string& descr);

    const std::string MOTION_DET_STR = "-motion_analysis";
};

#endif // ANALYSISSETTINGS_H
