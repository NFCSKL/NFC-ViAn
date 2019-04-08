#include "videointerval.h"

#include "Project/project.h"
#include "Project/videoproject.h"

#include <QDebug>

VideoInterval::VideoInterval(const int& start, const int& end,
                             VideoProject* vid_proj) {
    m_vid_proj_id = vid_proj->get_id();
    m_start = start;
    m_end = end;
    m_last_frame = vid_proj->get_video()->get_last_frame();
    m_state = vid_proj->state;
}

VideoInterval::VideoInterval() {
}

int VideoInterval::get_index() {
    return m_index;
}

int VideoInterval::get_start() {
    return m_start;
}

int VideoInterval::get_end() {
    return m_end;
}

int VideoInterval::get_last_frame() {
    return m_last_frame;
}

int VideoInterval::get_proj_id() {
    return m_vid_proj_id;
}

VideoProject* VideoInterval::get_vid_proj() {
    return m_proj->get_video_project(m_vid_proj_id);
}

Project* VideoInterval::get_proj() {
    return m_proj;
}

VideoState VideoInterval::get_state() {
    return m_state;
}

void VideoInterval::set_start(int start) {
    m_start = start;
}

void VideoInterval::set_end(int end) {
    m_end = end;
}

void VideoInterval::set_index(int index) {
    m_index = index;
}

void VideoInterval::set_project(Project* proj) {
    m_proj = proj;
}

bool VideoInterval::is_saved() const{
    return !m_unsaved_changes;
}

void VideoInterval::read(const QJsonObject& json) {
    m_vid_proj_id = json["vid_proj id"].toInt();
    m_start = json["start"].toInt();
    m_end = json["end"].toInt();
    m_index = json["index"].toInt();
    m_last_frame = json["last frame"].toInt();

    VideoState state;
    QJsonObject json_state = json["state"].toObject();
    state.read(json_state);
    m_state = state;

    m_unsaved_changes = false;
}

void VideoInterval::write(QJsonObject& json) {
    json["vid_proj id"] = m_vid_proj_id;
    json["start"] = m_start;
    json["end"] = m_end;
    json["index"] = m_index;
    json["last frame"] = m_last_frame;

    QJsonObject json_state;
    m_state.write(json_state);
    json["state"] = json_state;

    m_unsaved_changes = false;
}
