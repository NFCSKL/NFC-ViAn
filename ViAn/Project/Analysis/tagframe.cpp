#include "tagframe.h"
#include <QDebug>

TagFrame::TagFrame(int frame) {
    m_frame = frame;
    name = QString::number(frame);
}

TagFrame::TagFrame(int frame, VideoState state) {
    m_frame = frame;
    m_state = state;
    name = QString::number(frame);
}

TagFrame::~TagFrame() {
}

void TagFrame::update_color_correction(int b, double c, double g) {
    m_state.brightness = b;
    m_state.contrast = c;
    m_state.gamma = g;
}

void TagFrame::set_name(QString new_name) {
    name = new_name;
}

void TagFrame::set_frame(int frame) {
    m_state.frame = frame;
}

void TagFrame::set_scale_rot(double scale, int rot) {
    m_state.scale_factor = scale;
    m_state.rotation = rot;
}

void TagFrame::set_center(QPoint center) {
    m_state.center = center;
}

/**
 * @brief TagFrame::get_state
 * Make a copy and return
 * To prevent any update to the state.
 * @return
 */
VideoState TagFrame::get_state() {
    VideoState state;
    state = VideoState(m_state);
    return state;
}

VideoState TagFrame::get_original() {
    return m_state;
}

void TagFrame::read(const QJsonObject &json) {
    VideoState state;
    QJsonObject json_state = json["state"].toObject();
    state.read(json_state);
    m_state = state;
    name = json["name"].toString();
}

void TagFrame::write(QJsonObject &json) {
    QJsonObject json_state;
    m_state.write(json_state);
    json["state"] = json_state;
    json["frame"] = m_state.frame;
    json["name"] = name;
}
