#ifndef TAG_H
#define TAG_H
#include "analysis.h"
class Tag : public BasicAnalysis
{
public:
    Tag();
    bool add_frame(int frame);
    void remove_frame(int frame);
    virtual void add_interval(AnalysisInterval *an_interval);
private:
    void merge_intervals();
};

#endif // TAG_H
