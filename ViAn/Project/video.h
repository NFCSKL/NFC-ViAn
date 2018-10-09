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
    cv::Rect zoom_rect;
    VideoState(){}
    VideoState(VideoState&rh){
        frame = rh.frame;
        contrast = rh.contrast;
        brightness = rh.brightness;
        rotation = rh.rotation;
        scale_factor = rh.scale_factor;
        anchor = rh.anchor;
        center = rh.center;
        zoom_rect = rh.zoom_rect;
    }
};

typedef int ID;
class Video : Writeable{
protected:
    std::string m_name;
    int m_sequence_type;
public:
    VideoState state;
public:
    Video(const int& sequence_type=0);
    Video(std::string m_file_path, const int& sequence_type=0);
    ~Video();
    std::string m_file_path;
    std::string get_name();
    void set_name(const std::string& new_name);
    int get_sequence_type();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
