#ifndef TAG_H
#define TAG_H
#include "basicanalysis.h"
class BasicAnalysis;

class Tag : public BasicAnalysis {
    std::set<int> m_frames;

public:
    virtual ANALYSIS_TYPE get_type() const override;
    bool add_frame(int);
    bool remove_frame(int);
    std::set<int> get_frames();
};

#endif // TAG_H
