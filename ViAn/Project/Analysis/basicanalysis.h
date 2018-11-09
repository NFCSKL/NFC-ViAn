#ifndef BASICANALYSIS_H
#define BASICANALYSIS_H

#include "analysisinterval.h"
#include "Filehandler/saveable.h"

#include <QJsonObject>
#include <QMetaType>

#include <map>
#include <set>
#include <string>

class AnalysisSettings;

enum ANALYSIS_TYPE {MOTION_DETECTION = 1, TAG = 2, BASIC_ANALYSIS = 3, DRAWING_TAG = 4};

// TODO remove?
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
    AnalysisSettings* settings = nullptr;

private:
    ID id = 0;

protected:

public:
    BasicAnalysis();
    BasicAnalysis(const BasicAnalysis& other);
    virtual ~BasicAnalysis();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    virtual void add_interval(AnalysisInterval *ai);
    virtual ANALYSIS_TYPE get_type() const;

    ID get_id();
    void set_id(ID id);

    void clear_intervals();
    std::string get_name() const;
    interval_set get_intervals() const;

    void set_name(const std::string& new_name);

    bool is_saved() const;
};

Q_DECLARE_METATYPE(BasicAnalysis)
#endif // BASICANALYSIS_H
