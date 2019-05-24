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
    this->type = type;
    reset_settings();
}

AnalysisSettings::AnalysisSettings() {
    reset_settings();
}

AnalysisSettings::AnalysisSettings(AnalysisSettings* as) {
    type = as->type;
    reset_settings();
    set_full_settings(as->get_full_settings());
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

    // Add motion detection settings
    add_motion_setting("OPEN_DEGREE", 4, "Noise filtering, higher -> less noise");
    add_motion_setting("SMALLEST_OBJECT_SIZE", 100, "Smallest detected object");
    add_motion_setting("BACKGROUND_HISTORY", 500, "Number of frames in background model");
    add_motion_setting("MOG2_THRESHOLD", 10, "MOG2. Threshold on the squared Mahalanobis distance between the pixel and the model to decide whether a pixel is well described by the background model.");
    add_motion_setting("DETECT_SHADOWS", 0, "Detect shadows");
    add_motion_setting("SAMPLE_FREQUENCY", 1, "How often analysis will use frame from video");
    // Add object detection settings
    add_object_setting("Network size", 416, "Size of network's inut image");
    add_object_setting("Sample frequency (frames/sec)", 1, "How many frames to analyze per second");
    add_object_setting("Confidence threshold", 0.5, "Confidence threshold");
    add_object_setting("Nms threshold", 0.4, "Non-maximum suppression threshold");
}

void AnalysisSettings::add_motion_setting(const std::string &var, double value_default, const std::string& descr) {
        m_settings_motion[var] = value_default;
        m_descriptions_motion[var] = descr;
}

void AnalysisSettings::add_object_setting(const std::string &var, double value_default, const std::string& descr) {
        m_settings_object[var] = value_default;
        m_descriptions_object[var] = descr;
}

std::vector<std::string> AnalysisSettings::get_motion_var_names() {
    std::vector<std::string> res;
    for(auto pair : m_settings_motion){
        res.push_back(pair.first);
    }
    return res;
}

std::vector<std::string> AnalysisSettings::get_object_var_names() {
    std::vector<std::string> res;
    for(auto pair : m_settings_object){
        res.push_back(pair.first);
    }
    return res;
}

void AnalysisSettings::set_setting(const std::string &var, double value) {
    if (type == MOTION_DETECTION) {
        m_settings_motion[var] = value;
    } else if (type == OBJECT_DETECTION) {
        m_settings_object[var] = value;
    }
}

void AnalysisSettings::set_full_settings(Settings new_motion_settings, Settings new_object_settings) {
    m_settings_motion = new_motion_settings;
    m_settings_object = new_object_settings;
}

void AnalysisSettings::set_full_settings(std::pair<Settings, Settings> pair) {
    m_settings_motion = pair.first;
    m_settings_object = pair.second;
}

std::pair<Settings, Settings> AnalysisSettings::get_full_settings() {
    return std::make_pair(m_settings_motion, m_settings_object);
}

double AnalysisSettings::get_motion_setting(const std::string &var) {
    auto val_pair = m_settings_motion.find(var);
    if(val_pair != m_settings_motion.end())
        return val_pair->second;
    qWarning("No variable \"%s found",var.c_str());
    return -1;
}

double AnalysisSettings::get_object_setting(const std::string &var) {
    auto val_pair = m_settings_object.find(var);
    if(val_pair != m_settings_object.end())
        return val_pair->second;
    qWarning("No variable \"%s found",var.c_str());
    return -1;
}

std::string AnalysisSettings::get_descr(const std::string& var_name) {
    if (type == MOTION_DETECTION) {
        auto val_pair = m_settings_motion.find(var_name);
        if(val_pair != m_settings_motion.end()) {
            return m_descriptions_motion[val_pair->first];
        }
    } else if (type == OBJECT_DETECTION) {
        auto val_pair = m_settings_object.find(var_name);
        if(val_pair != m_settings_object.end()) {
            return m_descriptions_object[val_pair->first];
        }
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
    case OBJECT_DETECTION:
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
