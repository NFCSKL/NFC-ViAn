#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <vector>
#include "saveable.h"
enum ANALYSIS_TYPE{MOVEMENT_DETECTION = std::string("MOVEMENT_DETECTEN"), FACE_DETECTION = std::string("FACE_DETECTION")};
struct POI : Saveable{
    int start_frame;
    int end_frame;
    std::pair<double, double> upper_left;
    std::pair down_right;
    POI(std::pair<double,double> upper_left, std::pair<double,double> upper_left);
    POI(std::pair<double,double> upper_left, double height, double width);
};

class Analysis : Saveable{
    std::vector<POI> pois;
public:
    ANALYSIS_TYPE type;
    Analysis();
    void add_poi(POI poi);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);

};

#endif // ANALYSIS_H
