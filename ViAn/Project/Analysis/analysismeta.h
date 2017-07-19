#ifndef ANALYSISMETA_H
#define ANALYSISMETA_H
#include "analysis.h"
#include "tag.h"
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
    int type;
    std::string m_name;
    std::string file_analysis;
    std::vector<POI*> m_poi_intervals;
    AnalysisMeta(const Analysis &analysis);
    AnalysisMeta();
    Analysis *get_analysis(); // Only use this if all analysisinformation is needed
    AnalysisMeta(const AnalysisMeta &other);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    std::vector<POI *> getIntervals() const;
};
Q_DECLARE_METATYPE(AnalysisMeta)
#endif // ANALYSISMETA_H
