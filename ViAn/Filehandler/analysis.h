#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <vector>
#include "saveable.h"
enum ANALYSIS_TYPE {MOVEMENT = 0, FACE = 1};
class POI : Saveable{
    int frame;
    std::string file_path;
    std::pair<int, int> upper_left;
    std::pair<int, int> down_right;
public:
    POI();
    POI(std::pair<int,int> upper_left, std::pair<int,int> down_right);
    POI(std::pair<int,int> upper_left, int height, int width);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
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
