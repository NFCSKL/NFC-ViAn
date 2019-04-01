#ifndef TAGFRAME_H
#define TAGFRAME_H

#include "Filehandler/writeable.h"
#include "Project/video.h"

#include <QJsonObject>

class TagFrame : public Writeable {
    VideoState m_state;

public:
    TagFrame(int frame);
    TagFrame(int frame, VideoState state);
    virtual ~TagFrame();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);

    void update_color_correction(int b, double c, double g);
    void set_name(QString new_name);
    void set_frame(int frame);
    void set_scale_rot(double scale, int rot, bool flip_h, bool flip_v);
    void set_center(QPoint center);
    VideoState get_state();
    VideoState get_original();

    int m_frame;
    QString name;
};

#endif // TAGFRAME_H
