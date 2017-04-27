#include "analysis.h"

POI::POI(){}

void POI::add_detections(int frame_num, std::vector<OOI> detections) {
    if (start_frame == -1)
        start_frame = frame_num;
    OOIs[frame_num] = detections;
}

void POI::set_end_frame(int frame_num) {
    end_frame = frame_num;
}

void POI::read(const QJsonObject& json) {

}

void POI::write(QJsonObject& json) {
    json["start"] = start_frame;
    json["end"] = end_frame;
    QJsonArray json_OOIs;
    for(const auto& ooi_pair : OOIs){
        QJsonArray json_frame_OOIs;
        int frame = ooi_pair.first();
        std::vector<OOI> oois = ooi_pair.second;
        for (OOI o : oois) {

        }
        p.write(json_POI);
        json_POIs.append(json_POI);
    }
    json["POI:s"] = json_POIs;
}

OOI::OOI(){}

OOI::OOI(std::pair<int, int> upper_left, std::pair<int, int> down_right){
    this->upper_left = upper_left;
    this->lower_right = down_right;
}

OOI::OOI(std::pair<int, int> upper_left, int height, int width){
    this->upper_left = upper_left;
    this ->lower_right = std::make_pair(upper_left.second + width, upper_left.second + height);
}

OOI::OOI(cv::Rect rect) {
    upper_left = std::make_pair(rect.x, rect.y);
    lower_right = std::make_pair(rect.x + rect.width, rect.y + rect.height);
}

void OOI::read(const QJsonObject &json){
    this->frame = json["frame"].toDouble();

    this->upper_left.first = json["UL_X"].toDouble();
    this->upper_left.second = json["UL_Y"].toDouble();

    this->lower_right.first = json["DR_X"].toDouble();
    this->lower_right.second = json["DR_Y"].toDouble();
}

void OOI::write(QJsonObject &json){
    json["frame"] = this->frame;

    json["UL_X"] = this->upper_left.first;
    json["UL_Y"] = this->upper_left.second;

    json["DR_X"] = this->lower_right.first;
    json["DR_Y"] = this->lower_right.second;
}

Analysis::Analysis() {
}

Analysis::Analysis(const Analysis &obj) {
    POIs = obj.POIs;
    type = obj.type;
}

Analysis::~Analysis() {
}

void Analysis::add_POI(POI poi){
    this->POIs.push_back(poi);
}

void Analysis::read(const QJsonObject &json){
    this->type = (ANALYSIS_TYPE)json["analysis"].toInt();
    QJsonArray json_POIs = json["POI:s"].toArray();
    for (int i = 0; i < json_POIs.size(); ++i) {
        QJsonObject json_POI = json_POIs[i].toObject();
        POI POI;
        POI.read(json_POI);
        this->add_POI(POI);
    }
}

void Analysis::write(QJsonObject &json){
    json["analysis"] = this->type;
    QJsonArray json_POIs;
    for(auto it = this->POIs.begin(); it != this->POIs.end(); it++){
        QJsonObject json_POI;
        POI p = *it;
        p.write(json_POI);
        json_POIs.append(json_POI);
    }
    json["POI:s"] = json_POIs;
}
