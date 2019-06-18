#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H

#include "Project/Analysis/basicanalysis.h"

#include <opencv2/core/core.hpp>

using Settings = std::map<std::string,double>;
using SettingsDescr = std::map<std::string,std::string>;


/**
 * @brief The AnalysisSettings class
 * This class is used to configure
 * settings for analysismethod.
 */
class AnalysisSettings {
    Settings m_settings_motion;                // Custom integer settings for constants
    SettingsDescr m_descriptions_motion;       // Descriptions for settings constants
    Settings m_settings_object;
    SettingsDescr m_descriptions_object;

public:
    int type = MOTION_DETECTION;
    cv::Rect bounding_box;
    std::pair<int, int> interval;
    bool use_bounding_box = false;
    bool use_interval = false;
    bool quick_analysis = false;
    int frame_rate = 25;

    AnalysisSettings(int type, std::pair<int, int> interval, cv::Rect bounding_box);
    AnalysisSettings(int type);
    AnalysisSettings();
    AnalysisSettings(AnalysisSettings *as);
    virtual ~AnalysisSettings();

    void reset_settings();
    cv::Rect get_bounding_box() const;
    void set_bounding_box(const cv::Rect &value);
    int get_type() const;
    std::pair<int, int> get_interval() const;
    void set_interval(const std::pair<int, int> &value);

    std::vector<std::string> get_motion_var_names();
    std::vector<std::string> get_object_var_names();
    void set_setting(const std::string &var, double value);
    void set_full_settings(Settings new_motion_settings, Settings new_object_settings);
    void set_full_settings(std::pair<Settings, Settings> pair);
    std::pair<Settings, Settings> get_full_settings();
    double get_motion_setting(const std::string& var);
    double get_object_setting(const std::string& var);
    std::string get_descr(const std::string& var_name);
    std::string get_type_string();

protected:
    void add_motion_setting(const std::string& var, double value_default, const std::string& descr);
    void add_object_setting(const std::string& var, double value_default, const std::string& descr);
};

#endif // ANALYSISSETTINGS_H
