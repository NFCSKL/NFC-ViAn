#ifndef VIDEO_H
#define VIDEO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QString>
#include "Filehandler/saveable.h"
struct VideoState{
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

    int m_total_frames{}; // Total amount of frames in the video
    int m_total_time{};   // Total video length in ms
public:
    VideoState state;
std::string file_path;
public:
    Video();
    Video(std::string file_path);
    Video(std::string file_path, const int& total_frames, const int& total_time);


    std::string get_name();
    int get_frames();
    int get_time();

    void set_frames(const int& num_frames);
    void set_time(const int& time_ms);

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
