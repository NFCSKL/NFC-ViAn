#ifndef DRAWINGTAG_H
#define DRAWINGTAG_H

#include "basicanalysis.h"

class BasicAnalysis;
class DrawingTag : public BasicAnalysis {
    std::set<int> m_frames;

public:
    virtual ANALYSIS_TYPE get_type() const override;
    void add_frame(int frame);
    void remove_frame(int frame);
    std::set<int> get_frames();
};

#endif // DRAWINGTAG_H
