#include "drawingtag.h"

void DrawingTag::add_frame(int frame) {
    BasicAnalysis::add_interval(new AnalysisInterval(frame, frame));
}

void DrawingTag::remove_frame(int frame) {
    Q_UNUSED(frame)
}

ANALYSIS_TYPE DrawingTag::get_type() const {
    return DRAWING_TAG;
}
