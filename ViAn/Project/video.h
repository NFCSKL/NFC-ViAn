#ifndef VIDEO_H
#define VIDEO_H

#include "constants.h"
#include "Filehandler/saveable.h"

#include "opencv2/core/core.hpp"

#include <QJsonObject>
#include <QPoint>

struct VideoState : Writeable {
    int frame = 0;
    double contrast = Constants::CONTRAST_DEFAULT;
    int brightness = Constants::BRIGHTNESS_DEFAULT;
    double gamma = Constants::GAMMA_DEFAULT;
    int rotation = 0;
    bool flip_h = false;
    bool flip_v = false;
    double scale_factor = -1;
    QPoint anchor = QPoint(0,0);
    // Center point is relative the rotation
    QPoint center = QPoint(-1,-1);
    bool video = false;
    VideoState(){}
    VideoState(VideoState&rh){
        frame = rh.frame;
        contrast = rh.contrast;
        brightness = rh.brightness;
        gamma = rh.gamma;
        rotation = rh.rotation;
        flip_h = rh.flip_h;
        flip_v = rh.flip_v;
        scale_factor = rh.scale_factor;
        anchor = rh.anchor;
        center = rh.center;
        video = rh.video;
    }
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};

enum VIDEO_TYPE {VIDEO, VIDEO_SEQUENCE, TAG_SEQUENCE};

typedef int ID;
class Video : Writeable{
protected:
    QString m_name;
    VIDEO_TYPE m_sequence_type;
    bool m_is_saved{false};
    int m_width, m_height = 0;
    int frame_rate;
    int m_last_frame = 0;
public:
    VideoState state;
public:
    Video(const VIDEO_TYPE &sequence_type=VIDEO);
    Video(QString file_path, const VIDEO_TYPE &sequence_type=VIDEO);
    ~Video();
    QString file_path;
    QString get_name();
    void set_name(const QString &new_name);
    bool is_sequence();
    VIDEO_TYPE get_sequence_type();
    bool is_saved();
    int get_width();
    int get_height();
    int get_last_frame();
    void set_size(int width, int height);
    void set_frame_rate(int fps);
    int get_frame_rate();
    void set_last_frame(int frame);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
