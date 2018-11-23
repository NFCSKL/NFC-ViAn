#include "basicanalysis.h"

#include <QJsonArray>
#include <QJsonObject>

BasicAnalysis::BasicAnalysis() {}

BasicAnalysis::BasicAnalysis(const BasicAnalysis &other) :
    m_name(other.m_name),
    m_intervals(other.m_intervals),
    settings(other.settings)
{
}

BasicAnalysis::~BasicAnalysis() {
}

/**
 * @brief BasicAnalysis::add_interval
 * Adds an interval to the BasicAnalysis.
 * @param poi
 */
void BasicAnalysis::add_interval(AnalysisInterval *ai){
    m_intervals.insert(ai);
    m_unsaved_changes = true;
}

void BasicAnalysis::clear_intervals() {
    m_intervals.clear();
}

ANALYSIS_TYPE BasicAnalysis::get_type() const {
    return BASIC_ANALYSIS;
}

/**
 * @brief BasicAnalysis::read
 * Reads analysis from json format.
 * @param json
 */
void BasicAnalysis::read(const QJsonObject &json){
    m_name = json["name"].toString();
    QJsonArray json_intervals = json["POI:s"].toArray();
    for (int i = 0; i < json_intervals.size(); ++i) {
        QJsonObject json_interval = json_intervals[i].toObject();
        AnalysisInterval* interval = new AnalysisInterval();
        interval->read(json_interval);
        this->add_interval(interval);
    }
    m_unsaved_changes = false;
}

/**
 * @brief BasicAnalysis::write
 * Writes analysis to json format.
 * @param json
 */
void BasicAnalysis::write(QJsonObject &json){
    json["name"] = m_name;
    QJsonArray json_ais;
    for(auto it = this->m_intervals.begin(); it != this->m_intervals.end(); it++){
        QJsonObject json_ai;
        AnalysisInterval* p = *it;
        p->write(json_ai);
        json_ais.append(json_ai);
    }
    json["POI:s"] = json_ais;
    m_unsaved_changes = false;
}

ID BasicAnalysis::get_id() {
    return id;
}

void BasicAnalysis::set_id(ID new_id) {
    id = new_id;
}

QString BasicAnalysis::get_name() const {
    return m_name;
}

interval_set BasicAnalysis::get_intervals() const {
    return m_intervals;
}

void BasicAnalysis::set_name(const QString &new_name){
    m_name = new_name;
    m_unsaved_changes = true;
}

bool BasicAnalysis::is_saved() const{
    return !m_unsaved_changes;
}
