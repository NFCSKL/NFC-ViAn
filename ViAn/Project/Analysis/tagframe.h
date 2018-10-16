#ifndef TAGFRAME_H
#define TAGFRAME_H

#include <QJsonObject>
#include "Project/video.h"
#include "Filehandler/writeable.h"

class TagFrame : public Writeable {

public:
    TagFrame(int frame);
    TagFrame(int frame, VideoState state);
    virtual ~TagFrame();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);

    void update_color_correction(int b, double c, double g);

    VideoState m_state;
    int m_frame;
};

#endif // TAGFRAME_H
