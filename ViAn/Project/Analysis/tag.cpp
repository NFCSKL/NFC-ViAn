#include "tag.h"

Tag::Tag(std::string name, bool drawing_tag) {
    m_name = name;
    m_drawing_tag = drawing_tag;
}

Tag::~Tag() {
    for (auto it = tag_map.begin(); it != tag_map.end(); ++it) {
        delete (*it).second;
    }
    tag_map.clear();
}

void Tag::add_frame(int frame, TagFrame* t_frame) {
    m_unsaved_changes = true;
    tag_map.insert(std::pair<int,TagFrame*>(frame, t_frame));
}

bool Tag::find_frame(int frame) {
    return tag_map.count(frame);
}

void Tag::remove_frame(int frame) {
    auto it = tag_map.find(frame);
    if (it != tag_map.end()) {
        tag_map.erase(it);
        delete (*it).second;
        m_unsaved_changes = true;
    }
}

void Tag::update_color_correction(int frame, int b_value, double c_value) {
    auto it = tag_map.find(frame);
    if (it != tag_map.end()) {
        (*it).second->update_color_correction(b_value, c_value);
        m_unsaved_changes = true;
    }
}

int Tag::next_frame(int frame) {
    auto it = tag_map.upper_bound(frame);
    if (it != tag_map.end()) {
        return (*it).first;
    } else {
        return frame;
    }
}

int Tag::previous_frame(int frame) {
    auto it = tag_map.lower_bound(frame);
    if (it != tag_map.end() && it != tag_map.begin()) {
        return (*(std::prev(it))).first;
    } else if (it == tag_map.end() && tag_map.size() >= 1) {
        return (*(std::prev(it))).first;
    } else {
        return frame;
    }
}

std::vector<int> Tag::get_frames() {
    std::vector<int> frames;
    for (auto pair : tag_map) {
        frames.push_back(pair.first);
    }
    return frames;
}

bool Tag::is_drawing_tag() {
    return m_drawing_tag;
}

void Tag::set_drawing_tag(bool value) {
    m_drawing_tag = value;
}

ANALYSIS_TYPE Tag::get_type() const {
    if (m_drawing_tag) return DRAWING_TAG;
    return TAG;
}

void Tag::write(QJsonObject &json) {
    json["name"] = QString::fromStdString(m_name);
    json["drawing_tag"] = m_drawing_tag;
    QJsonArray frames;
    for (auto tag_frame : tag_map) {
        QJsonObject f_num;
        tag_frame.second->write(f_num);
        frames.push_back(f_num);
    }
    json["frames"] = frames;
    m_unsaved_changes = false;
}

void Tag::read(const QJsonObject &json) {
    m_name = json["name"].toString().toStdString();
    m_drawing_tag = json["drawing_tag"].toBool();
    QJsonArray json_intervals = json["frames"].toArray();
    for (int i = 0; i < json_intervals.size(); ++i) {
        QJsonObject json_frame = json_intervals[i].toObject();
        int frame = json_frame["frame"].toInt();
        TagFrame* t_frame = new TagFrame(frame);
        t_frame->read(json_frame);
        tag_map.insert(std::pair<int,TagFrame*>(frame, t_frame));
    }
    m_unsaved_changes = false;
}
