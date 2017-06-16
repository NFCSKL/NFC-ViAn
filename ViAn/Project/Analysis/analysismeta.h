#ifndef ANALYSISMETA_H
#define ANALYSISMETA_H
#include "analysis.h"
#include "Filehandler/saveable.h"
class AnalysisMeta : public Saveable
{
    std::vector<std::pair<int,int>> m_poi_intervals;
public:
    AnalysisMeta(Analysis& analysis);
    AnalysisMeta(const std::string &file_path);
    Analysis get_analysis();

    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);


};

#endif // ANALYSISMETA_H
