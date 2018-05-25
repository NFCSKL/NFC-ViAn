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
    // Fix videostate loading
    VideoState state;

    //m_state(-1, 0, 0, 0, 1, QPoint(0,0));
    m_frame = json["frame"].toInt();
    state.frame = m_frame;
    state.scale_factor = json["scale_factor"].toDouble();
    int x = json["anchor x"].toInt();
    int y = json["anchor y"].toInt();
    state.zoom_start = QPoint(x, y);
    m_state = state;
}

void TagFrame::write(QJsonObject &json) {
    json["frame"] = m_frame;
    json["scale_factor"] = m_state.scale_factor;
    json["anchor x"] = m_state.zoom_start.x();
    json["anchor y"] = m_state.zoom_start.y();
}
