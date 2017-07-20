#include "basicanalysis.h"

BasicAnalysis::BasicAnalysis()
{

}

BasicAnalysis::BasicAnalysis(const BasicAnalysis &other) :
    type(other.type),
    m_name(other.m_name),
    m_intervals(other.m_intervals)
{

}
/**
 * @brief BasicAnalysis::add_POI
 * Adds a POI to the BasicAnalysis.
 * @param poi
 */
void BasicAnalysis::add_interval(AnalysisInterval *ai){
    m_intervals.insert(ai);
}

/**
 * @brief BasicAnalysis::read
 * Reads analysis from json format.
 * @param json
 */
void BasicAnalysis::read(const QJsonObject &json){
    this->type = (ANALYSIS_TYPE)json["type"].toInt();
    this->m_name = json["name"].toString().toStdString();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        AnalysisInterval* poi = new AnalysisInterval();
        poi->read(json_poi);
        this->add_interval(poi);
    }
}

/**
 * @brief BasicAnalysis::write
 * Writes analysis to json format.
 * @param json
 */
void BasicAnalysis::write(QJsonObject &json){
    json["type"] = this->type;
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

std::string BasicAnalysis::get_name() const
{
    return m_name;
}

int BasicAnalysis::get_type() const
{
    return type;
}

interval_set BasicAnalysis::get_interval() const
{
    return m_intervals;
}
