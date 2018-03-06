#include "basicanalysis.h"
#include "tag.h"
#include "analysis.h"
#include "drawingtag.h"

BasicAnalysis::BasicAnalysis() {}

BasicAnalysis::BasicAnalysis(const BasicAnalysis &other) :
    m_name(other.m_name),
    m_intervals(other.m_intervals),
    m_ana_interval(other.m_ana_interval),
    bounding_box(other.bounding_box),
    use_interval(other.use_interval),
    use_bounding_box(other.use_bounding_box){
}

/**
 * @brief BasicAnalysis::add_POI
 * Adds a POI to the BasicAnalysis.
 * @param poi
 */
void BasicAnalysis::add_interval(AnalysisInterval *ai){
    m_intervals.insert(ai);
}

void BasicAnalysis::clear_intervals() {
    m_intervals.clear();
}

SAVE_TYPE BasicAnalysis::get_save_type() const
{
    return INTERVAL;
}

ANALYSIS_TYPE BasicAnalysis::get_type() const
{
    return BASIC_ANALYSIS;
}

/**
 * @brief BasicAnalysis::read
 * Reads analysis from json format.
 * @param json
 */
void BasicAnalysis::read(const QJsonObject &json){
    m_name = json["name"].toString().toStdString();
    QJsonArray json_intervals = json["POI:s"].toArray();
    for (int i = 0; i < json_intervals.size(); ++i) {
        QJsonObject json_interval = json_intervals[i].toObject();
        AnalysisInterval* interval = new AnalysisInterval();
        interval->read(json_interval);
        this->add_interval(interval);
    }
}

/**
 * @brief BasicAnalysis::write
 * Writes analysis to json format.
 * @param json
 */
void BasicAnalysis::write(QJsonObject &json){
    json["name"] = QString::fromStdString(m_name);
    QJsonArray json_ais;
    for(auto it = this->m_intervals.begin(); it != this->m_intervals.end(); it++){
        QJsonObject json_ai;
        AnalysisInterval* p = *it;
        p->write(json_ai);
        json_ais.append(json_ai);
    }
    json["POI:s"] = json_ais;
}

std::string BasicAnalysis::get_name() const {
    return m_name;
}

interval_set BasicAnalysis::get_intervals() const {
    return m_intervals;
}

std::pair<int, int> BasicAnalysis::get_ana_interval() const {
    return m_ana_interval;
}

cv::Rect BasicAnalysis::get_bounding_box() const {
    return bounding_box;
}
