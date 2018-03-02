#ifndef ANALYSISPROXY_H
#define ANALYSISPROXY_H
#include "basicanalysis.h"
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
class AnalysisProxy : public BasicAnalysis
{
    std::string file_analysis = "";  // m_analysis.full_path()
    ANALYSIS_TYPE type;
public:
    AnalysisProxy();
    AnalysisProxy(const std::string file_analysis);
    Analysis *load_analysis(); // Only use this if all analysisinformation is needed
    AnalysisProxy(const Analysis &other, const std::string file);
    AnalysisProxy(const AnalysisProxy &other);

    void reset_root_dir(const std::string& dir);
    virtual ANALYSIS_TYPE get_type() const override;
    virtual SAVE_TYPE get_save_type() const override;
    virtual std::string full_path() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject& json) override;
};
Q_DECLARE_METATYPE(AnalysisProxy)
#endif // ANALYSISPROXY_H
