#ifndef VIDEO_H
#define VIDEO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QString>
#include "Filehandler/saveable.h"
#include <opencv2/opencv.hpp>

struct VideoState{
    int frame = -1;
    int contrast = 0;
    int brightness = 0;
    int rotation = 0;
    double scale_factor = 1;
    cv::Rect zoom_rect;
    VideoState(){}
    VideoState(VideoState&rh){
        frame = rh.frame;
        contrast = rh.contrast;
        brightness = rh.brightness;
        rotation = rh.rotation;
        scale_factor = rh.scale_factor;
        zoom_rect = rh.zoom_rect;
    }
};

typedef int ID;
class Video : Writeable{
    std::string m_name;
public:
    VideoState state;
public:
    Video();
    Video(std::string file_path);
    ~Video();
    std::string file_path;
    std::string get_name();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
