#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include "saveable.h"
#include "opencv2/core/core.hpp"
#include <iostream>
enum ANALYSIS_TYPE {MOTION_DETECTION = 0, FACIAL_DETECTION = 1};
const std::vector<std::string> ANALYSIS_NAMES = {"Motion detection", "Facial detection"};
const std::map<std::string, ANALYSIS_TYPE> ANALYSIS_NAMES_TYPE_MAP = {std::make_pair("Motion detection",MOTION_DETECTION),
                                                                     std::make_pair("Facial detection",FACIAL_DETECTION)};

class OOI : Saveable{
    int frame;
    std::string file_path;
    std::pair<int, int> upper_left;
    std::pair<int, int> lower_right;
public:
    OOI();
    OOI(std::pair<int,int> upper_left, std::pair<int,int> lower_right);
    OOI(std::pair<int,int> upper_left, int height, int width);
    OOI(cv::Rect rect);

    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    cv::Rect get_rect();
    void scale_coordinates(float scale_ratio);
};

class POI : Saveable{
    std::map<int,std::vector<OOI>> OOIs;
public:
    POI();
    int start_frame = -1;
    int end_frame = -1;
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void add_detections(int frame_num, std::vector<OOI> detections);
    void set_end_frame(int frame_num);
    friend class Analysis;
};

class Analysis : Saveable {
public:
    ANALYSIS_TYPE type;
    Analysis();
    ~Analysis();
    Analysis(const Analysis &obj);

    void add_POI(POI POI);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    std::vector<POI> POIs;
};
Q_DECLARE_METATYPE(Analysis);

#endif // ANALYSIS_H
