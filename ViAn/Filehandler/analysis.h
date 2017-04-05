#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <vector>
#include "video.h"
#include "Video/shapes/analysarea.h"
typedef size_t Frame; // Defined for readability
class Video;

struct POI{
    AnalysArea area; // Area of interest
    Frame start;    // First frame of interest
    Frame end;      // Last frame of interest
    POI(AnalysArea area, Frame start, Frame end){
        this->area = area;
        this->start = start;
        this->end = end;
    }
};

class Analysis{
    Video* analyzed_video;                  // Video which analysis applies too
    std::vector<POI*> points_of_interest;   // Points of interest

public:    
    Analysis(Video *analyzed_video);
    void add_poi(AnalysArea rect, Frame start, Frame end); // POI:s are added continuosly
};

#endif // ANALYSIS_H
