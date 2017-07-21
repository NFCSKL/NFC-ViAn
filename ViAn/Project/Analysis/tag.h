#ifndef TAG_H
#define TAG_H
#include "basicanalysis.h"
class BasicAnalysis;
class Tag : public BasicAnalysis
{
public:
    bool add_frame(int frame);
    void remove_frame(int frame);
    void add_interval(AnalysisInterval *an_interval) override;
    virtual ANALYSIS_TYPE get_type() const override;
private:
    void merge_intervals();
};

#endif // TAG_H
