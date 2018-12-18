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

void TagFrame::read(const QJsonObject &json) {
    VideoState state;
    m_frame = json["frame"].toInt();
    state.frame = m_frame;
    state.scale_factor = json["scale_factor"].toDouble();
    int x = json["anchor x"].toInt();
    int y = json["anchor y"].toInt();
    state.anchor = QPoint(x, y);
    x = json["center_x"].toInt();
    y = json["center_y"].toInt();
    state.center = QPoint(x, y);
    state.rotation = json["rotation"].toInt();
    state.brightness = json["brightness"].toInt();
    state.contrast = json["contrast"].toDouble();
    state.gamma = json["gamma"].toDouble();
    m_state = state;
}

void TagFrame::write(QJsonObject &json) {
    json["frame"] = m_frame;
    json["scale_factor"] = m_state.scale_factor;
    json["anchor x"] = m_state.anchor.x();
    json["anchor y"] = m_state.anchor.y();
    json["center_x"] = m_state.center.x();
    json["center_y"] = m_state.center.y();
    json["rotation"] = m_state.rotation;
    json["brightness"] = m_state.brightness;
    json["contrast"] = m_state.contrast;
    json["gamma"] = m_state.gamma;
}
