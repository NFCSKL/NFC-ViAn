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

enum ANALYSIS_TYPE {MOTION_DETECTION = 1, TAG, BASIC_ANALYSIS, DRAWING_TAG, INTERVAL,
                    SEQUENCE_TAG, OBJECT_DETECTION};

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
    QString m_name = "2";
    bool m_unsaved_changes = true;
    interval_set m_intervals;
    AnalysisSettings* settings = nullptr;

private:
    ID id = 0;
    ID vid_proj_id = -1;

protected:

public:
    BasicAnalysis();
    BasicAnalysis(const BasicAnalysis& other);
    virtual ~BasicAnalysis();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    virtual void add_interval(AnalysisInterval *ai);
    virtual int get_type() const;

    ID get_id();
    void set_id(ID id);
    ID get_vid_proj_id();
    void set_vid_proj_id(ID id);

    void clear_intervals();
    QString get_name() const;
    interval_set get_intervals() const;

    void set_name(const QString& new_name);

    bool is_saved() const;
};

Q_DECLARE_METATYPE(BasicAnalysis)
#endif // BASICANALYSIS_H
