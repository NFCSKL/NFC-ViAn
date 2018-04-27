#ifndef VIDEO_H
#define VIDEO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QString>
#include "Filehandler/saveable.h"
struct VideoState{
    int total_frames{}; // Total amount of frames in the video
    int frame{};        // Frame where the playback is at
    int contrast = 0;
    int brightness = 0;
    int rotation = 0;
    VideoState(){}
    VideoState(VideoState&rh){
        frame = rh.frame;
        contrast = rh.contrast;
        brightness = rh.brightness;
        rotation = rh.rotation;
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
    std::string file_path;
    std::string get_name();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
