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
using ID = int;
class BasicAnalysis : public Saveable
{       
public:
    // TODO Should probably not all be public
    std::string m_name = "2";
    bool m_unsaved_changes = true;
    interval_set m_intervals;
    std::pair<int, int> m_ana_interval = std::make_pair(-1, -1);
    cv::Rect bounding_box;
    bool use_interval = false;
    bool use_bounding_box = false;

private:
    ID id = 0;

protected:

public:

    BasicAnalysis();
    BasicAnalysis(const BasicAnalysis& other);
    ~BasicAnalysis();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    virtual void add_interval(AnalysisInterval *ai);
    virtual SAVE_TYPE get_save_type() const;
    virtual ANALYSIS_TYPE get_type() const;

    ID get_id();
    void set_id(ID id);

    std::string get_name() const;
    interval_set get_intervals() const;

    void set_name(const std::string& new_name);

    bool is_saved() const;
    std::pair<int, int> get_ana_interval() const;
    cv::Rect get_bounding_box() const;
};

Q_DECLARE_METATYPE(BasicAnalysis)
#endif // BASICANALYSIS_H
