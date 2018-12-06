#include "videostate.h"

VideoState::VideoState() {

}

void VideoState::read(const QJsonObject &json) {
    frame = json["frame"].toInt();
    scale_factor = json["scale_factor"].toDouble();
    int x = json["anchor x"].toInt();
    int y = json["anchor y"].toInt();
    anchor = QPoint(x, y);
    x = json["center_x"].toInt();
    y = json["center_y"].toInt();
    center = QPoint(x, y);
    rotation = json["rotation"].toInt();
    brightness = json["brightness"].toInt();
    contrast = json["contrast"].toDouble();
    gamma = json["gamma"].toDouble();
}

void VideoState::write(QJsonObject &json) {
    json["frame"] = frame;
    json["scale_factor"] = scale_factor;
    json["anchor x"] = anchor.x();
    json["anchor y"] = anchor.y();
    json["center x"] = center.x();
    json["center y"] = center.y();
    json["rotation"] = rotation;
    json["brightness"] = brightness;
    json["contrast"] = contrast;
    json["gamma"] = gamma;
}
