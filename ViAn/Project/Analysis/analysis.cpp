#include "analysis.h"

/**
 * @brief Analysis::Analysis
 */
Analysis::Analysis() {
}

/**
 * @brief Analysis::Analysis
 * Copy constructor. Needed for signals and slots.
 * @param obj
 */
Analysis::Analysis(const Analysis &obj) {
    POIs = obj.POIs;
    type = obj.type;
    name = obj.name;
}

/**
 * @brief Analysis::set_name
 * @param QString name
 */
void Analysis::set_name(QString name){
    this->name = name;
}

/**
 * @brief Analysis::~Analysis
 */
Analysis::~Analysis() {
}

/**
 * @brief Analysis::add_POI
 * Adds a POI to the analysis.
 * @param poi
 */
void Analysis::add_POI(POI poi){
    this->POIs.push_back(poi);
}

/**
 * @brief Analysis::read
 * Reads analysis from json format.
 * @param json
 */
void Analysis::read(const QJsonObject &json){
    this->type = (ANALYSIS_TYPE)json["type"].toInt();
    this->name = json["name"].toString();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        POI poi;
        poi.read(json_poi);
        this->add_POI(poi);
    }
}

/**
 * @brief Analysis::write
 * Writes analysis to json format.
 * @param json
 */
void Analysis::write(QJsonObject &json){
    json["type"] = this->type;
    json["name"] = this->name;
    QJsonArray json_POIs;
    for(auto it = this->POIs.begin(); it != this->POIs.end(); it++){
        QJsonObject json_POI;
        POI p = *it;
        p.write(json_POI);
        json_POIs.append(json_POI);
    }
    json["POI:s"] = json_POIs;
}

/**
 * @brief Analysis::get_detections_on_frame
 * Returns all detections on a specified frame in the analysed video.
 * @param frame_num
 * @return
 */
std::vector<cv::Rect> Analysis::get_detections_on_frame(int frame_num) {
    std::vector<cv::Rect> rects;
    for (POI p : POIs) {
        if (frame_num >= p.start_frame && frame_num <= p.end_frame) {
            std::vector<OOI> oois = p.OOIs[frame_num];
            for (OOI o : oois) {
                rects.push_back(o.get_rect());
            }
            break;
        }
    }
    return rects;
}
