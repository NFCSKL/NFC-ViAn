#include "tagframe.h"
#include <QDebug>

TagFrame::TagFrame(int frame) {
    m_frame = frame;
}

TagFrame::TagFrame(int frame, VideoState state) {
    m_frame = frame;
    m_state = state;
}

TagFrame::~TagFrame() {

}

void TagFrame::read(const QJsonObject &json) {
    VideoState state;
    m_frame = json["frame"].toInt();
    state.frame = m_frame;
    state.scale_factor = json["scale_factor"].toDouble();
    int x = json["anchor x"].toInt();
    int y = json["anchor y"].toInt();
    state.anchor = QPoint(x, y);
    state.brightness = json["brightness"].toInt();
    state.contrast = json["contrast"].toDouble();
    m_state = state;
}

void TagFrame::write(QJsonObject &json) {
    json["frame"] = m_frame;
    json["scale_factor"] = m_state.scale_factor;
    json["anchor x"] = m_state.anchor.x();
    json["anchor y"] = m_state.anchor.y();
    json["brightness"] = m_state.brightness;
    json["contrast"] = m_state.contrast;
}
