#include "tag.h"

bool Tag::add_frame(int frame) {
    return m_frames.insert(frame).second;
}

bool Tag::remove_frame(int frame) {
    auto it = m_frames.find(frame);
    if (it != m_frames.end()) {
        m_frames.erase(it);
        return true;
    }
    return false;
}

std::set<int> Tag::get_frames() {
    return m_frames;
}

ANALYSIS_TYPE Tag::get_type() const {
    return TAG;
}
