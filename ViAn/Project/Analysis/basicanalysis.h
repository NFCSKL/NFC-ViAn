#ifndef BASICANALYSIS_H
#define BASICANALYSIS_H
#include <string>
#include <utility>
#include <set>
#include "analysisinterval.h"
#include "opencv2/core/core.hpp"
#include <typeinfo>
#include <QPoint>
#include <QJsonArray>
enum ANALYSIS_TYPE {MOTION_DETECTION = 1, TAG =2, BASIC_ANALYSIS =3};
enum SAVE_TYPE {DETECTION=10, INTERVAL=11};
const std::map<std::string, ANALYSIS_TYPE> ANALYSIS_NAMES_TYPE_MAP = {std::make_pair("Motion detection",MOTION_DETECTION),
                                                                     std::make_pair("Tag",TAG)};
struct interval_cmp {
    bool operator()(const AnalysisInterval* lhs, const AnalysisInterval* rhs) const {
        return lhs->get_start() <= rhs->get_start();
    }
};
using interval_set = std::set<AnalysisInterval*, interval_cmp>;
class BasicAnalysis : public Saveable
{       

public:
    std::string m_name = "2";
    AnalysisInterval m_ana_interval;
    cv::Rect bounding_box;
    QPoint rect_start, rect_end;
    bool use_interval = false;
    bool use_bounding_box = false;


protected:
    interval_set m_intervals;
public:

    BasicAnalysis();
    BasicAnalysis(const BasicAnalysis& other);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    virtual void add_interval(AnalysisInterval *ai);
    virtual SAVE_TYPE get_save_type() const;
    virtual ANALYSIS_TYPE get_type() const;

    std::string get_name() const;
    interval_set get_intervals() const;
    AnalysisInterval get_ana_interval() const;
    cv::Rect get_bounding_box() const;
    QPoint get_rect_start() const;
    QPoint get_rect_end() const;

};

Q_DECLARE_METATYPE(BasicAnalysis)
#endif // BASICANALYSIS_H
