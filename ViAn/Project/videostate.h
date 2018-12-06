#ifndef VIDEOSTATE_H
#define VIDEOSTATE_H

#include "constants.h"
#include "Filehandler/saveable.h"

struct VideoState : Writeable
{
    int frame = 0;
    double contrast = Constants::CONTRAST_DEFAULT;
    int brightness = Constants::BRIGHTNESS_DEFAULT;
    double gamma = Constants::GAMMA_DEFAULT;
    int rotation = 0;
    double scale_factor = 1;
    QPoint anchor = QPoint(0,0);
    // Center point is relative the rotation
    QPoint center = QPoint(-1,-1);
    bool video = false;
    // Some kind of video
    VideoState(){}
    VideoState(VideoState&rh){
        frame = rh.frame;
        contrast = rh.contrast;
        brightness = rh.brightness;
        gamma = rh.gamma;
        rotation = rh.rotation;
        scale_factor = rh.scale_factor;
        anchor = rh.anchor;
        center = rh.center;
        video = rh.video;
    }
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};

#endif // VIDEOSTATE_H
