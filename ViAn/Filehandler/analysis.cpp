#include "analysis.h"

/**
 * @brief Analysis::Analysis
 * @param analyzed_video
 */
Analysis::Analysis(Video* analyzed_video){
    this->analyzed_video = analyzed_video;
}

/**
 * @brief Analysis::add_poi
 * @param area
 * @param start
 * @param end
 * Add new poi.
 */
void Analysis::add_poi(AnalysArea area, Frame start, Frame end){
    POI* poi = new POI(area, start, end);
    this->points_of_interest.push_back(poi);
}
