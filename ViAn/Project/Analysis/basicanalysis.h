#ifndef BASICANALYSIS_H
#define BASICANALYSIS_H
#include <string>
#include <utility>
#include <set>
#include "analysisinterval.h"
#include <typeinfo>
#include <QJsonArray>
//#include "Project/videoproject.h"

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
//    VideoProject* m_v_proj = nullptr;
public:
    std::string m_name = "";
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

    void changed();
//    void set_video_project(VideoProject* v_proj);
};

Q_DECLARE_METATYPE(BasicAnalysis)
#endif // BASICANALYSIS_H
