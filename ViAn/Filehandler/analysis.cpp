#include "analysis.h"

Analysis::Analysis(Video* analyzed_video){
    this->analyzed_video = analyzed_video;
}

void Analysis::add_poi(Rectangle rect, Frame start, Frame end){
    POI poi = new POI(rect, start, end);
    this->points_of_interest.push_back(poi);
}
