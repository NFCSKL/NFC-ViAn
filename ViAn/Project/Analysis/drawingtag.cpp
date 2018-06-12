#include "drawingtag.h"

void DrawingTag::add_frame(int frame) {
    m_frames.insert(frame);
}

void DrawingTag::remove_frame(int frame) {
    Q_UNUSED(frame)
}

std::set<int> DrawingTag::get_frames() {
    return m_frames;
}

ANALYSIS_TYPE DrawingTag::get_type() const {
    return DRAWING_TAG;
}
