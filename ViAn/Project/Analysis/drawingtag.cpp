#include "drawingtag.h"

void DrawingTag::add_frame(int frame) {
    //add_interval(new AnalysisInterval(frame, frame));
    BasicAnalysis::add_interval(new AnalysisInterval(frame, frame));
}

void DrawingTag::remove_frame(int frame) {
    //remove_interval(new AnalysisInterval(frame, frame));
}

ANALYSIS_TYPE DrawingTag::get_type() const {
    return DRAWING_TAG;
}
