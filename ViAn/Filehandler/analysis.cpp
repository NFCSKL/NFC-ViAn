#include "analysis.h"

POI::POI(){
    this->upper_left = std::make_pair<double,double>;
    this->upper_right = std::make_pair<double,double>;
}

POI::POI(std::pair<double, double> upper_left, std::pair<double, double> down_right){
    this->upper_left = upper_left;
    this->down_right = down_right;
}

POI::POI(std::pair<double, double> upper_left, double height, double width){
    this->upper_left = upper_left;
    this ->down_right = std::make_pair(upper_left.second + width, upper_left.second + height);
}

Analysis::Analysis()
{
    this->pois.clear();
}

void Analysis::add_poi(POI poi){
    this->pois.push_back(poi);
}

void Analysis::read(const QJsonObject &json){
    this->type = json["analysis"].toString().toStdString();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        POI poi;
        poi.read(json_pois);
        v->read(json_poi);
        this->add_poi(poi);
    }
}

void Analysis::write(QJsonObject &json){
    json["analysis"] = QString(this->type);
    QJsonArray json_pois;
    for(auto it = this->pois.begin(); it != this->pois; it++){
        QJsonObject json_poi;
        POI p = *it;
        p.write(json_poi);
        json_pois.append(json_poi);
    }
    json["POI:s"] = json_pois;
}
