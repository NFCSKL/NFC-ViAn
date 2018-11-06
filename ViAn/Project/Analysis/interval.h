#ifndef INTERVAL_H
#define INTERVAL_H

#include "basicanalysis.h"

#include <QJsonObject>

#include <vector>

class Interval : public BasicAnalysis
{
public:
    Interval(std::string name);
    ~Interval() override;
    virtual ANALYSIS_TYPE get_type() const override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject& json) override;

    void add_area(int start, int end);
    void add_area(std::pair<int, int> interval);
    //std::vector<std::pair<int, int>> get_area_list();

    std::vector<std::pair<int, int>> m_area_list;

//public:
//    void remove_interval(AnalysisInterval *rm_interval);
//    void add_frame(int frame);
//    void remove_frame(int frame);
//    void add_interval(AnalysisInterval *an_interval) override;
//
//private:
//    void merge_intervals();
//    bool is_interval(int start, int end);
//    std::pair<int, int> get_overlap(AnalysisInterval *intval1, AnalysisInterval *intval2);
//    std::pair<AnalysisInterval *, AnalysisInterval *> remove_overlap(AnalysisInterval *interval, std::pair<int, int> overlap);
};

#endif // INTERVAL_H
