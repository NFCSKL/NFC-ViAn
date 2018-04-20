#ifndef DRAWINGTAG_H
#define DRAWINGTAG_H

#include "basicanalysis.h"

class BasicAnalysis;
class DrawingTag : public BasicAnalysis
{

public:
    void add_frame(int frame);
    void remove_frame(int frame);
    virtual ANALYSIS_TYPE get_type() const override;
private:

};

#endif // DRAWINGTAG_H
