#include "analysismeta.h"
#include <QJsonObject>
#include <QString>

/**
 * @brief AnalysisMeta::AnalysisMeta
 * @param analysis
 */
AnalysisMeta::AnalysisMeta(const Analysis &analysis)
{
    m_name = analysis.name;
    m_full_path = analysis.m_full_path;
    std::vector<POI> pois = analysis.POIs;
    std::pair<int,int> pair;
    POI poi;
    for (auto it = pois.begin(); it != pois.end(); ++it) {
        poi = *it;
        pair = std::make_pair(poi.start_frame, poi.end_frame);
        m_poi_intervals.push_back(pair);
    }
}

/**
 * @brief AnalysisMeta::AnalysisMeta
 */
AnalysisMeta::AnalysisMeta()
{
}

/**
 * @brief AnalysisMeta::get_analysis
 * @return
 */
Analysis AnalysisMeta::get_analysis()
{
    Analysis analysis;
    analysis.load_saveable(m_full_path);
    return analysis;
}

/**
 * @brief AnalysisMeta::AnalysisMeta
 * @param other
 */
AnalysisMeta::AnalysisMeta(const AnalysisMeta &other)
{
    m_name = other.m_name;
    m_poi_intervals = other.m_poi_intervals;    
    m_full_path = other.m_full_path;
}

/**
 * @brief AnalysisMeta::read
 * @param json
 */
void AnalysisMeta::read(const QJsonObject &json)
{
    m_name = json["name"].toString().toStdString();
    m_full_path = json["full_path"].toString().toStdString();
    QJsonArray json_intervals = json["intervals"].toArray();
    for (int i = 0; i < json_intervals.size() ; ++i) {
        QJsonObject json_pair = json_intervals[i].toObject();
        std::pair<int,int> pair;
        pair.first = json_pair["start"].toInt();
        pair.second = json_pair["end"].toInt();
        m_poi_intervals.push_back(pair);
    }
}

/**
 * @brief AnalysisMeta::write
 * @param json
 */
void AnalysisMeta::write(QJsonObject &json)
{
    json["name"] = QString::fromStdString(m_name);
    json["full_path"] = QString::fromStdString(m_full_path);
    QJsonArray intervals;
    for (auto it = m_poi_intervals.begin(); it != m_poi_intervals.end(); ++it) {
        QJsonObject interval;
        std::pair<int,int> pair = *it;
        interval["start"] = pair.first;
        interval["end"] = pair.second;
        intervals.push_back(interval);
    }
    json["intervals"] = intervals;
}
