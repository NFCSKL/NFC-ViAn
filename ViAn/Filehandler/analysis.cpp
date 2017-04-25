#include "analysis.h"

POI::POI(){}

POI::POI(std::pair<double, double> upper_left, std::pair<double, double> down_right){
    this->upper_left = upper_left;
    this->down_right = down_right;
}

POI::POI(std::pair<double, double> upper_left, double height, double width){
    this->upper_left = upper_left;
    this ->down_right = std::make_pair(upper_left.second + width, upper_left.second + height);
}

void POI::read(const QJsonObject &json){
    this->frame = json["frame"].toDouble();

    this->upper_left.first = json["UL_X"].toDouble();
    this->upper_left.second = json["UL_Y"].toDouble();

    this->down_right.first = json["DR_X"].toDouble();
    this->down_right.second = json["DR_Y"].toDouble();
}

void POI::write(QJsonObject &json){
    json["frame"] = this->frame;

    json["UL_X"] = this->upper_left.first;
    json["UL_Y"] = this->upper_left.second;

    json["DR_X"] = this->down_right.first;
    json["DR_Y"] = this->down_right.second;
}

Analysis::Analysis()
{
    this->pois.clear();
}

void Analysis::add_poi(POI poi){
    this->pois.push_back(poi);
}

void Analysis::read(const QJsonObject &json){
    this->type = (ANALYSIS_TYPE)json["analysis"].toInt();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        POI poi;
        poi.read(json_poi);
        this->add_poi(poi);
    }
}

void Analysis::write(QJsonObject &json){
    json["analysis"] = this->type;
    QJsonArray json_pois;
    for(auto it = this->pois.begin(); it != this->pois.end(); it++){
        QJsonObject json_poi;
        POI p = *it;
        p.write(json_poi);
        json_pois.append(json_poi);
    }
    json["POI:s"] = json_pois;
}
