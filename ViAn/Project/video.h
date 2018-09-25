#ifndef VIDEO_H
#define VIDEO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QString>
#include <QPoint>
#include "Filehandler/saveable.h"
#include "Video/framemanipulator.h"
#include <opencv2/opencv.hpp>

class ImageSequence;
struct VideoState {
    int frame = 0;
    double contrast = FrameManipulator().CONTRAST_DEFAULT;
    int brightness = FrameManipulator().BRIGHTNESS_DEFAULT;
    int rotation = 0;
    double scale_factor = 1;
    QPoint anchor = QPoint(0,0);
    // Center point is relative the rotation
    QPoint center = QPoint(-1,-1);
    VideoState(){}
    VideoState(VideoState&rh){
        frame = rh.frame;
        contrast = rh.contrast;
        brightness = rh.brightness;
        rotation = rh.rotation;
        scale_factor = rh.scale_factor;
        anchor = rh.anchor;
        center = rh.center;
    }
};

typedef int ID;
class Video : Writeable{
protected:
    std::string m_name;
    bool m_is_sequence;
    int m_width, m_height = 0;
public:
    VideoState state;
public:
    Video(const bool& is_sequence=false);
    Video(std::string file_path, const bool& is_sequence=false);
    ~Video();
    std::string file_path;
    std::string get_name();
    void set_name(const std::string& new_name);
    bool is_sequence();
    int get_width();
    int get_height();
    void set_size(int width, int height);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
