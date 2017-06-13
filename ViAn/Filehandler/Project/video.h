#ifndef VIDEO_H
#define VIDEO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <QJsonObject>
#include <QString>
#include "Filehandler/saveable.h"
typedef int ID;
class Video : Saveable{

public:
    Video();
    Video(std::string file_path);
    std::string file_path;
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    friend bool operator==(Video v1, Video v2);
};

#endif // VIDEO_H
