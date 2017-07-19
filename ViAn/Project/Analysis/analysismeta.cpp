#include "analysismeta.h"
#include <QJsonObject>
#include <QString>

/**
 * @brief AnalysisMeta::AnalysisMeta
 * @param analysis
 */
AnalysisMeta::AnalysisMeta(const Analysis &analysis) {
    type = analysis.type;
    m_name = analysis.name;
    file_analysis = analysis.full_path();
    std::vector<POI*> v(analysis.m_intervals.begin(), analysis.m_intervals.end());
    m_poi_intervals = v;
}

/**
 * @brief AnalysisMeta::AnalysisMeta
 */
AnalysisMeta::AnalysisMeta() {
}

/**
 * @brief AnalysisMeta::get_analysis
 * @return
 */
Analysis* AnalysisMeta::get_analysis() {
    Analysis* analysis = new Analysis();
    analysis->load_saveable(file_analysis);
    return analysis;
}

/**
 * @brief AnalysisMeta::AnalysisMeta
 * @param other
 */
AnalysisMeta::AnalysisMeta(const AnalysisMeta &other) {
    m_name = other.m_name;
    m_poi_intervals = other.m_poi_intervals;    
    file_analysis = other.file_analysis;
}

/**
 * @brief AnalysisMeta::read
 * @param json
 */
void AnalysisMeta::read(const QJsonObject &json) {
    m_name = json["name"].toString().toStdString();
    file_analysis = json["full_path"].toString().toStdString();
    QJsonArray json_intervals = json["intervals"].toArray();
    for (int i = 0; i < json_intervals.size() ; ++i) {
        QJsonObject json_pair = json_intervals[i].toObject();
        std::pair<int,int> pair;
        pair.first = json_pair["start"].toInt();
        pair.second = json_pair["end"].toInt();
        m_poi_intervals.push_back(new POI(pair.first, pair.second));
    }
}

/**
 * @brief AnalysisMeta::write
 * @param json
 */
void AnalysisMeta::write(QJsonObject &json) {
    json["name"] = QString::fromStdString(m_name);
    json["full_path"] = QString::fromStdString(file_analysis);
    QJsonArray intervals;
    for (auto it = m_poi_intervals.begin(); it != m_poi_intervals.end(); ++it) {
        QJsonObject interval;
        POI* pair = *it;
        interval["start"] = pair->getStart();
        interval["end"] = pair->getEnd();
        intervals.push_back(interval);
    }
    json["intervals"] = intervals;
}

std::vector<POI*> AnalysisMeta::getIntervals() const
{
    return m_poi_intervals;
}
