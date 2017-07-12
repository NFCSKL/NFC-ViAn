#include "analysis.h"

/**
 * @brief Analysis::Analysis
 */
std::string Analysis::get_name() const {
    return name;
}

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
void Analysis::set_name(const std::string& name){
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
void Analysis::add_POI(POI* poi){
    POIs.insert(poi);
}

/**
 * @brief Analysis::add_frame
 * @param frame
 * Adds the newly tagged frame as a new poi
 * unless it's at the edge of a current poi or that frame already is tagged
 */
void Analysis::add_frame(int frame) {
    //for (auto it = POIs.begin(); it != POIs.end(); ++it) {
    for (auto p : POIs) {
        if (p->is_in_POI(frame) || p->at_edge(frame)) return;
    }
    POI* poi = new POI();
    poi->start_frame = frame;
    poi->end_frame = frame;
    add_POI(poi);
}

/**
 * @brief Analysis::read
 * Reads analysis from json format.
 * @param json
 */
void Analysis::read(const QJsonObject &json){
    this->type = (ANALYSIS_TYPE)json["type"].toInt();
    this->name = json["name"].toString().toStdString();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        POI* poi = new POI();
        poi->read(json_poi);
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
    json["name"] = QString::fromStdString(this->name);
    QJsonArray json_POIs;
    for(auto it = this->POIs.begin(); it != this->POIs.end(); it++){
        QJsonObject json_POI;
        POI* p = *it;
        p->write(json_POI);
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
    for (auto p : POIs) {
        if (frame_num >= p->start_frame && frame_num <= p->end_frame) {
            std::vector<OOI> oois = p->OOIs[frame_num];
            for (OOI o : oois) {
                rects.push_back(o.get_rect());
            }
            break;
        }
    }
    return rects;
}
