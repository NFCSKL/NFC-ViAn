#ifndef ANALYSISPROXY_H
#define ANALYSISPROXY_H

#include "basicanalysis.h"

#include <QJsonObject>

class Analysis;
class AnalsysSettings;

/**
 * @brief The AnalysisProxy class
 * This class is used to improve performance of basic
 * operations related to analyses. Full analysis is therefor
 * only fully loaded upon direct request.
 */
class AnalysisProxy : public BasicAnalysis
{
    QString file_analysis = "";  // m_analysis.full_path()
    ANALYSIS_TYPE type = MOTION_DETECTION; //TODO Remove
public:
    AnalysisProxy();
    AnalysisProxy(const QString file_analysis);
    AnalysisProxy(const Analysis &other, const QString file);
    AnalysisProxy(const AnalysisProxy &other);
    ~AnalysisProxy() override;
    Analysis *load_analysis(); // Only use this if all analysisinformation is needed

    std::vector<std::pair<int, int>> m_slider_interval;
    bool is_new = false;

    AnalysisSettings *get_settings();

    void reset_root_dir(const QString &dir);
    virtual ANALYSIS_TYPE get_type() const override;
    virtual QString full_path() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject& json) override;
};
Q_DECLARE_METATYPE(AnalysisProxy)
#endif // ANALYSISPROXY_H
