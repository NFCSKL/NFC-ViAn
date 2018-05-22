#include "tag.h"

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
    } else {
        return frame;
    }
}

std::map<int,TagFrame*> Tag::get_frames() {
    return tag_map;
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
    for (auto pair : tag_map) {
        int frame = pair.first;
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
        TagFrame* t_frame = new TagFrame(frame);
        tag_map.insert(std::pair<int,TagFrame*>(frame, t_frame));
    }
    m_unsaved_changes = false;
}
