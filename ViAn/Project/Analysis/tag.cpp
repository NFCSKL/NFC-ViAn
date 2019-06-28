#include "tag.h"

#include "tagframe.h"

#include <QDebug>
#include <QJsonArray>

#include <map>

Tag::Tag(QString name, int type) {
    m_name = name;
    m_type = type;
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
        delete (*it).second;
        tag_map.erase(it);
        m_unsaved_changes = true;
    }
}

/**
 * @brief Tag::update_color_correction
 * Update the current tag_frame's color correction
 * @param frame     : Frame number of the tag_frame
 * @param b_value   : brightness value
 * @param c_value   : contrast value
 */
void Tag::update_color_correction(int frame, int b_value, double c_value, double g_value) {
    auto it = tag_map.find(frame);
    if (it != tag_map.end()) {
        (*it).second->update_color_correction(b_value, c_value, g_value);
        m_unsaved_changes = true;
    }
}

/**
 * @brief Tag::update_color_whole_tag
 * Updates the color correction on all tag_frames in the current tag
 * @param b     : brightness value
 * @param c     : contrast value
 */
void Tag::update_color_whole_tag(int b, double c, double g) {
    for (auto it = tag_map.begin(); it != tag_map.end(); ++it) {
        (*it).second->update_color_correction(b, c, g);
    }
}

/**
 * @brief Tag::update_index_tag
 * Update the tag so all tag frames' index matches the files index.
 * Use after changing a frame of a tag_sequence
 */
void Tag::update_index_tag() {
    if (m_type != SEQUENCE_TAG) return;
    std::map<int, TagFrame*> temp_map;
    int index = 0;
    for (auto it = tag_map.begin(); it != tag_map.end(); ++it) {
        if (it->first != index) {
            it->second->set_frame(index);
            temp_map[index] = it->second;
        } else {
            temp_map[index] = it->second;
        }
        index++;
    }
    tag_map = temp_map;
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
    return (m_type == DRAWING_TAG);
}

void Tag::revert_tag_map() {
    tag_map = saved_map;
}

int Tag::get_type() const {
    return m_type;
}

void Tag::write(QJsonObject &json) {
    json["name"] = m_name;
    json["type"] = m_type;
    QJsonArray frames;
    for (auto tag_frame : tag_map) {
        QJsonObject f_num;
        tag_frame.second->write(f_num);
        frames.push_back(f_num);
    }
    saved_map = tag_map;
    json["frames"] = frames;
    m_unsaved_changes = false;
}

void Tag::read(const QJsonObject &json) {
    m_name = json["name"].toString();
    m_type = json["type"].toInt();
    QJsonArray json_intervals = json["frames"].toArray();
    for (int i = 0; i < json_intervals.size(); ++i) {
        QJsonObject json_frame = json_intervals[i].toObject();
        int frame = json_frame["frame"].toInt();
        TagFrame* t_frame = new TagFrame(frame);
        t_frame->read(json_frame);

        tag_map.insert(std::pair<int,TagFrame*>(frame, t_frame));
    }
    saved_map = tag_map;
    m_unsaved_changes = false;
}
