#include "analysisproxy.h"

#include "analysis.h"
#include "Analysis/analysissettings.h"
#include "poi.h"
#include "utility.h"

#include "opencv2/core/core.hpp"

#include <QJsonArray>
#include <QJsonObject>

/**
 * @brief AnalysisProxy::AnalysisProxy
 */
AnalysisProxy::AnalysisProxy() {}

AnalysisProxy::AnalysisProxy(const QString file_analysis) :
    file_analysis(file_analysis) {
}

AnalysisProxy::AnalysisProxy(const Analysis &other, const QString file)
    : BasicAnalysis(other),
      file_analysis(file),
      type(other.type) {
}

AnalysisProxy::AnalysisProxy(const AnalysisProxy &other) :
    BasicAnalysis(other),
    file_analysis(other.file_analysis),
    type(other.type) {
}

AnalysisProxy::~AnalysisProxy() {
    m_slider_interval.clear();
}

/**
 * @brief AnalysisProxy::load_analysis
 * @return
 */
Analysis* AnalysisProxy::load_analysis() {
    Analysis* analysis = new Analysis();
    analysis->load_saveable(file_analysis);
    m_unsaved_changes = false;
    return analysis;
}

AnalysisSettings* AnalysisProxy::get_settings() {
    return settings;
}

void AnalysisProxy::reset_root_dir(const QString &dir) {
    file_analysis = dir+Utility::name_from_path(file_analysis);
    m_unsaved_changes = true;
}


/**
 * @brief AnalysisProxy::read
 * @param json
 */
void AnalysisProxy::read(const QJsonObject &json) {
    AnalysisSettings* new_settings = new AnalysisSettings();
    m_name = json["name"].toString();
    int height = json["bounding box height"].toInt();
    int width = json["bounding box width"].toInt();
    int y = json["bounding box y"].toInt();
    int x = json["bounding box x"].toInt();
    new_settings->bounding_box = cv::Rect(x, y, width, height);
    new_settings->interval = std::make_pair(json["interval start"].toInt(), json["interval end"].toInt());

    std::vector<std::string> vars = new_settings->get_var_names();
    for (std::string name : vars) {
        new_settings->set_setting(name, json[QString::fromStdString(name)].toInt());
    }
    settings = new_settings;

    file_analysis = json["full_path"].toString();
    QJsonArray json_intervals = json["intervals"].toArray();
    for (int i = 0; i < json_intervals.size() ; ++i) {
        QJsonObject json_poi = json_intervals[i].toObject();
        POI* poi = new POI();
        poi->read(json_poi);
        std::pair<int, int> pair = std::make_pair(poi->get_start(), poi->get_end());
        m_slider_interval.push_back(pair);
        m_intervals.insert(poi);
    }
    m_unsaved_changes = false;
}

/**
 * @brief AnalysisProxy::write
 * @param json
 */
void AnalysisProxy::write(QJsonObject &json) {
    json["name"] = m_name;
    json["bounding box height"] = settings->bounding_box.height;
    json["bounding box width"] = settings->bounding_box.width;
    json["bounding box y"] = settings->bounding_box.y;
    json["bounding box x"] = settings->bounding_box.x;
    json["interval start"] = settings->interval.first;
    json["interval end"] = settings->interval.second;

    std::vector<std::string> vars = settings->get_var_names();
    for (std::string name : vars) {
        json[QString::fromStdString(name)] = settings->get_setting(name);
    }

    json["full_path"] = file_analysis;
    QJsonArray intervals;
    for (auto it = m_slider_interval.begin(); it != m_slider_interval.end(); ++it) {
        QJsonObject interval;
        interval["start"] = (*it).first;
        interval["end"] = (*it).second;
        intervals.push_back(interval);
    }
    json["intervals"] = intervals;
    m_unsaved_changes = false;
}

int AnalysisProxy::get_type() const {
    return type;
}

QString AnalysisProxy::full_path() const {
    return file_analysis;
}
