#ifndef TAGFRAME_H
#define TAGFRAME_H

#include "Filehandler/writeable.h"
#include "Project/video.h"

#include <QJsonObject>

class TagFrame : public Writeable {

public:
    TagFrame(int frame);
    TagFrame(int frame, VideoState state);
    virtual ~TagFrame();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);

    void update_color_correction(int b, double c);

    VideoState m_state;
    int m_frame;
};

#endif // TAGFRAME_H
