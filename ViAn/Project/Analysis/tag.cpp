#include "tag.h"

Tag::~Tag() {
    m_frames.clear();
}

bool Tag::add_frame(int frame) {
    m_unsaved_changes = true;
    return m_frames.insert(frame).second;
}

void Tag::add_frame_state(int frame, VideoState state) {
    new_tags.insert(frame, state);
}

bool Tag::remove_frame(int frame) {
    auto it = m_frames.find(frame);
    if (it != m_frames.end()) {
        m_frames.erase(it);
        return m_unsaved_changes = true;
    }
    return false;
}

int Tag::next_frame(int frame) {
    auto it = m_frames.upper_bound(frame);
    if (it != m_frames.end()) {
        return *it;
    } else {
        return frame;
    }
}

int Tag::previous_frame(int frame) {
    qDebug() << m_frames.size();
    auto it = m_frames.lower_bound(frame);
    if (it != m_frames.end() && it != m_frames.begin()) {
        return *(std::prev(it));
    } else {
        return frame;
    }
}

std::set<int> Tag::get_frames() {
    return m_frames;
}

bool Tag::is_drawing_tag() {
    return drawing_tag;
}

void Tag::set_drawing_tag(bool value) {
    drawing_tag = value;
}

ANALYSIS_TYPE Tag::get_type() const {
    if (drawing_tag) return DRAWING_TAG;
    return TAG;
}

void Tag::write(QJsonObject &json) {
    json["name"] = QString::fromStdString(m_name);
    json["drawing_tag"] = drawing_tag;
    QJsonArray frames;
    for (int frame : m_frames) {
        QJsonObject f_num;
        f_num["frame"] = frame;
        frames.push_back(f_num);
    }
    json["frames"] = frames;
    m_unsaved_changes = false;
}

void Tag::read(const QJsonObject &json) {
    m_name = json["name"].toString().toStdString();
    drawing_tag = json["drawing_tag"].toBool();
    QJsonArray json_intervals = json["frames"].toArray();
    for (int i = 0; i < json_intervals.size(); ++i) {
        QJsonObject json_frame = json_intervals[i].toObject();
        int frame = json_frame["frame"].toInt();
        m_frames.insert(frame);
    }
    m_unsaved_changes = false;
}
