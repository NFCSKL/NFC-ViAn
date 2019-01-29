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

void TagFrame::update_color_correction(int b, double c, double g) {
    m_state.brightness = b;
    m_state.contrast = c;
    m_state.gamma = g;
}

void TagFrame::read(const QJsonObject &json) {
    VideoState state;
    QJsonObject json_state = json["state"].toObject();
    state.read(json_state);
    m_state = state;
}

void TagFrame::write(QJsonObject &json) {
    QJsonObject json_state;
    m_state.write(json_state);
    json["state"] = json_state;
}
