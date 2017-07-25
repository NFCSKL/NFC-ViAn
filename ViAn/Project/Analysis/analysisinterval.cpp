#include "analysisinterval.h"

AnalysisInterval::AnalysisInterval(int start, int end)
{
    m_start = start;
    m_end = end;
}

AnalysisInterval::AnalysisInterval() {}

AnalysisInterval::~AnalysisInterval()
{

}

/**
 * @brief POI::read
 * Reads POI from json format.
 * @param json
 */
void AnalysisInterval::read(const QJsonObject& json) {
    this->m_start = json["start"].toInt();
    this->m_end = json["end"].toInt();
}

/**
 * @brief POI::write
 * Writes POI to json format.
 * @param json
 */
void AnalysisInterval::write(QJsonObject& json) {
    json["start"] = m_start;
    json["end"] = m_end;
}

/**
 * @brief AnalysisInterval::at_edge
 * Checks if frame_num is the next or previous frame of the POI
 * if it is, add it
 * @param frame_num
 * @return
 */
bool AnalysisInterval::at_edge(int frame_num) {
    if (frame_num == m_start-1) {
        m_start = frame_num;
        return true;
    } else if (frame_num == m_end+1) {
        m_end = frame_num;
        return true;
    }
    return false;
}

bool AnalysisInterval::in_interval(int frame) const
{
   return frame >= m_start && frame <= m_end;
}

int AnalysisInterval::get_start() const
{
    return m_start;
}

int AnalysisInterval::get_end() const
{
    return m_end;
}

AnalysisInterval::interval AnalysisInterval::get_interval()
{
    return std::make_pair(m_start, m_end);
}
