#include "analysisproxy.h"
#include <QJsonObject>
#include <QString>
#include "utility.h"

/**
 * @brief AnalysisMeta::AnalysisMeta
 */
AnalysisProxy::AnalysisProxy()
{
}

AnalysisProxy::AnalysisProxy(const std::string file_analysis) :
    file_analysis(file_analysis) {
}

/**
 * @brief AnalysisMeta::get_analysis
 * @return
 */
Analysis* AnalysisProxy::load_analysis() {
    Analysis* analysis = new Analysis();    
    analysis->load_saveable(file_analysis);
    return analysis;
}
AnalysisProxy::AnalysisProxy(const Analysis &other, const std::string file)
    : BasicAnalysis(other),
      file_analysis(file),
      type(other.type) {
}

AnalysisProxy::AnalysisProxy(const AnalysisProxy &other) :
    BasicAnalysis(other),
    file_analysis(other.file_analysis),
    type(other.type)
{
}

void AnalysisProxy::reset_root_dir(const std::string &dir) {
    file_analysis = dir+Utility::name_from_path(file_analysis);
}


/**
 * @brief AnalysisMeta::read
 * @param json
 */
void AnalysisProxy::read(const QJsonObject &json) {
    m_name = json["name"].toString().toStdString();
    file_analysis = json["full_path"].toString().toStdString();
    QJsonArray json_intervals = json["intervals"].toArray();
    for (int i = 0; i < json_intervals.size() ; ++i) {
        QJsonObject json_poi = json_intervals[i].toObject();
        POI* poi = new POI();
        poi->read(json_poi);
        m_intervals.insert(poi);
    }
}

/**
 * @brief AnalysisMeta::write
 * @param json
 */
void AnalysisProxy::write(QJsonObject &json) {
    json["name"] = QString::fromStdString(m_name);
    json["full_path"] = QString::fromStdString(file_analysis);
    QJsonArray intervals;
    for (auto it = m_intervals.begin(); it != m_intervals.end(); ++it) {
        QJsonObject interval;
        POI* pair = dynamic_cast<POI*>(*it);
        interval["start"] = pair->get_start();
        interval["end"] = pair->get_end();
        intervals.push_back(interval);
    }
    json["intervals"] = intervals;
}

SAVE_TYPE AnalysisProxy::get_save_type() const
{
    return DETECTION;
}

ANALYSIS_TYPE AnalysisProxy::get_type() const
{
    return type;
}

std::string AnalysisProxy::full_path() const
{
    return file_analysis;
}
