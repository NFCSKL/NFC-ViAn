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
bool Analysis::add_frame(int frame) {
    return frames.insert(frame).second;
}

void Analysis::remove_frame(int frame) {
    frames.erase(frame);
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
    if (type == TAG) {
        for (int i = 0; i < json_pois.size(); ++i) {
            QJsonObject json_tag = json_pois[i].toObject();
            frames.insert(json_tag["Frame"].toInt());
        }
    } else {
        for (int i = 0; i < json_pois.size(); ++i) {
            QJsonObject json_poi = json_pois[i].toObject();
            POI* poi = new POI();
            poi->read(json_poi);
            this->add_POI(poi);
        }
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
    if (!frames.empty()) {
        for(auto it = this->frames.begin(); it != this->frames.end(); it++){
            QJsonObject json_tag;
            json_tag["Frame"] = *it;
            json_POIs.append(json_tag);
        }

    } else {
        for(auto it = this->POIs.begin(); it != this->POIs.end(); it++){
            QJsonObject json_POI;
            POI* p = *it;
            p->write(json_POI);
            json_POIs.append(json_POI);
        }
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
