#ifndef ANALYSISMETA_H
#define ANALYSISMETA_H
#include "analysis.h"
#include "Filehandler/saveable.h"
#include <iostream>
class AnalysisMeta : public Saveable
{
    std::vector<std::pair<int,int>> m_poi_intervals;
    std::string file_analysis;
public:
    std::string m_name;
    AnalysisMeta(const Analysis &analysis);
    AnalysisMeta();
    Analysis get_analysis();
    AnalysisMeta(const AnalysisMeta &other);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};
Q_DECLARE_METATYPE(AnalysisMeta)
#endif // ANALYSISMETA_H
