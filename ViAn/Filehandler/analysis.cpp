#include "analysis.h"

Analysis::Analysis(Video* analyzed_video){
    this->analyzed_video = analyzed_video;
}

void Analysis::add_poi(AnalysArea area, Frame start, Frame end){
    POI* poi = new POI(area, start, end);
    this->points_of_interest.push_back(poi);
}
