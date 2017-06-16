#ifndef ANALYSISMETA_H
#define ANALYSISMETA_H
#include "analysis.h"
#include "Filehandler/saveable.h"
#include <iostream>
class AnalysisMeta : public Saveable
{
    std::vector<std::pair<int,int>> m_poi_intervals;
public:
    std::string m_name;
    AnalysisMeta(Analysis& analysis);
    AnalysisMeta();
    Analysis get_analysis();
    AnalysisMeta(const AnalysisMeta &other);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    virtual void delete_saveable();


};
Q_DECLARE_METATYPE(AnalysisMeta)
#endif // ANALYSISMETA_H
