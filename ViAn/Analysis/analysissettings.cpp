#include "analysissettings.h"

#include "constants.h"

AnalysisSettings::AnalysisSettings(int type,
                                   std::pair<int, int> interval,
                                   cv::Rect bounding_box)
    : type(type)
{
    set_interval(interval);
    set_bounding_box(bounding_box);
}

AnalysisSettings::AnalysisSettings(int type) : type(type) {
    reset_settings();
    this->type = type;
}

AnalysisSettings::AnalysisSettings() {
    reset_settings();
}

AnalysisSettings::AnalysisSettings(AnalysisSettings* as) {
    reset_settings();
    m_settings = as->get_full_settings();
    type = as->type;
    bounding_box = as->bounding_box;
    interval = as->interval;
    use_bounding_box = as->use_bounding_box;
    use_interval = as->use_interval;
    quick_analysis = as->quick_analysis;
}


AnalysisSettings::~AnalysisSettings() {}

void AnalysisSettings::reset_settings() {
    use_bounding_box = false;
    use_interval = false;
    type = MOTION_DETECTION;
    add_setting("OPEN_DEGREE", 4, "Noise filtering, higher -> less noise");
    add_setting("SMALLEST_OBJECT_SIZE", 100, "Smallest detected object");
    add_setting("BACKGROUND_HISTORY", 500, "Number of frames in background model");
    add_setting("MOG2_THRESHOLD", 10, "MOG2. Threshold on the squared Mahalanobis distance between the pixel and the model to decide whether a pixel is well described by the background model.");
    add_setting("DETECT_SHADOWS", 0, "Detect shadows");
    add_setting("SAMPLE_FREQUENCY", 1, "How often analysis will use frame from video");
}

void AnalysisSettings::add_setting(const std::string &var, int value_default, const std::string& descr) {
    m_settings[var] = value_default;
    m_descriptions[var] = descr;
}

std::vector<std::string> AnalysisSettings::get_var_names() {
    std::vector<std::string> res;
    for(auto pair : m_settings){
        res.push_back(pair.first);
    }
    return res;
}

void AnalysisSettings::set_setting(const std::string &var, int value) {
    m_settings[var] = value;
}

void AnalysisSettings::set_full_settings(Settings new_settings) {
    m_settings = new_settings;
}

Settings AnalysisSettings::get_full_settings() {
    return m_settings;
}

int AnalysisSettings::get_setting(const std::string &var) {
    auto val_pair = m_settings.find(var);
    if(val_pair != m_settings.end())
        return val_pair->second;
    qWarning("No variable \"%s found",var.c_str());
    return -1;
}

std::string AnalysisSettings::get_descr(const std::string& var_name) {
    auto val_pair = m_settings.find(var_name);
    if(val_pair != m_settings.end()) {
        return m_descriptions[val_pair->first];
    }
    qWarning("No variable \"%s found",var_name.c_str());
    return "";
}

std::string AnalysisSettings::get_type_string() {
    // Extend this switch when adding more types of analyses
    // for the correct string ending
    switch (type) {
    case MOTION_DETECTION:
        return Constants::MOTION_DET_STR;
    case YOLO:
        return Constants::YOLO_STR;
    default:
        return "";
    }
}

cv::Rect AnalysisSettings::get_bounding_box() const {
    return bounding_box;
}

void AnalysisSettings::set_bounding_box(const cv::Rect &value) {
    bounding_box = value;
    use_bounding_box = true;
}

int AnalysisSettings::get_type() const {
    return type;
}

std::pair<int, int> AnalysisSettings::get_interval() const {
    return interval;
}

void AnalysisSettings::set_interval(const std::pair<int, int> &value) {
    interval = value;
    use_interval = true;
}
