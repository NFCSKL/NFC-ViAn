#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <vector>
#include "../Video/shapes/rectangle.h"
#include "video.h"
typedef size_t Frame; // Defined for readability

struct POI{
    Rectangle area; // Area of interest
    Frame start;    // First frame of interest
    Frame end;      // Last frame of interest
    POI(Rectangle rect, Frame start, Frame end){
        this->area = rect;
        this->start = start;
        this->end = end;
    }
};

class Analysis{
    Video* analyzed_video;                  // Video which analysis applies too
    std::vector<POI*> points_of_interest;   // Points of interest

public:    
    Analysis(Video *analyzed_video);
    void add_poi(Rectangle rect, Frame start, Frame end); // POI:s are added continuosly
};

#endif // ANALYSIS_H
