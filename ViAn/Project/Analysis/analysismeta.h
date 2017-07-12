#ifndef ANALYSISMETA_H
#define ANALYSISMETA_H
#include "analysis.h"
#include "Filehandler/saveable.h"
#include <iostream>

/**
 * @brief The AnalysisMeta class
 * This class is used to improve performance of basic
 * operations related to analyses. Full analysis is therefor
 * only fully loaded upon direct request.
 */
class AnalysisMeta : public Saveable
{

public:
    std::string m_name;
    std::string file_analysis;
    std::vector<std::pair<int,int>> m_poi_intervals;
    AnalysisMeta(const Analysis &analysis);
    AnalysisMeta();
    Analysis get_analysis(); // Only use this if all analysisinformation is needed
    AnalysisMeta(const AnalysisMeta &other);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};
Q_DECLARE_METATYPE(AnalysisMeta)
#endif // ANALYSISMETA_H
