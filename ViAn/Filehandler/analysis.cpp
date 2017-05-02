#include "analysis.h"

/**
 * @brief POI::POI
 */
POI::POI(){}

/**
 * @brief POI::add_detections
 * Adds OOIs for a specific frame in a POI.
 * @param frame_num
 * @param detections
 */
void POI::add_detections(int frame_num, std::vector<OOI> detections) {
    if (start_frame == -1)
        start_frame = frame_num;
    OOIs[frame_num] = detections;
}

/**
 * @brief POI::set_end_frame
 * Sets the end frame of the POI.
 * @param frame_num
 */
void POI::set_end_frame(int frame_num) {
    end_frame = frame_num;
}

/**
 * @brief POI::read
 * Reads POI from json format.
 * @param json
 */
void POI::read(const QJsonObject& json) {
    this->start_frame = json["start"].toInt();
    this->end_frame = json["end"].toInt();    
    for(int i = start_frame; i != end_frame; i++){        
        QJsonArray json_frame_OOIs = json[QString::number(i)].toArray();
        std::vector<OOI> oois;
        for(int j  = 0; j = json_frame_OOIs.size(); j++){            
            OOI ooi;
            ooi.read(json_frame_OOIs[j].toObject());
            oois.push_back(ooi);
        }
        this->OOIs.insert(std::make_pair(i, oois));
    }
}

/**
 * @brief POI::write
 * Writes POI to json format.
 * @param json
 */
void POI::write(QJsonObject& json) {
    json["start"] = start_frame;
    json["end"] = end_frame;

    for(const auto& ooi_pair : OOIs){
        QJsonArray json_frame_OOIs;
        int frame = ooi_pair.first;
        std::vector<OOI> oois = ooi_pair.second;
        for (OOI o : oois) {
            QJsonObject json_ooi;
            o.write(json_ooi);
            json_frame_OOIs.append(json_ooi);
        }
        json[QString::number(frame)] = json_frame_OOIs;
    }
}

/**
 * @brief OOI::OOI
 */
OOI::OOI(){}

/**
 * @brief OOI::OOI
 * @param upper_left
 * @param down_right
 */
OOI::OOI(std::pair<int, int> upper_left, std::pair<int, int> down_right){
    this->upper_left = upper_left;
    this->lower_right = down_right;
}

/**
 * @brief OOI::OOI
 * @param upper_left
 * @param height
 * @param width
 */
OOI::OOI(std::pair<int, int> upper_left, int height, int width){
    this->upper_left = upper_left;
    this ->lower_right = std::make_pair(upper_left.second + width, upper_left.second + height);
}

/**
 * @brief OOI::OOI
 * Creates an OOI from a cv::Rect.
 * @param rect
 */
OOI::OOI(cv::Rect rect) {
    upper_left = std::make_pair(rect.x, rect.y);
    lower_right = std::make_pair(rect.x + rect.width, rect.y + rect.height);
}

/**
 * @brief OOI::read
 * Reads OOI from json format.
 * @param json
 */
void OOI::read(const QJsonObject &json){
    this->upper_left.first = json["UL_X"].toInt();
    this->upper_left.second = json["UL_Y"].toInt();

    this->lower_right.first = json["LR_X"].toInt();
    this->lower_right.second = json["LR_Y"].toInt();
}

/**
 * @brief OOI::write
 * Writes OOI to json format.
 * @param json
 */
void OOI::write(QJsonObject &json){
    json["UL_X"] = this->upper_left.first;
    json["UL_Y"] = this->upper_left.second;

    json["LR_X"] = this->lower_right.first;
    json["LR_Y"] = this->lower_right.second;
}

/**
 * @brief OOI::get_rect
 * Returns the rectangle specified by the OOI.
 * @return
 */
cv::Rect OOI::get_rect() {
    return cv::Rect(upper_left.first, upper_left.second, lower_right.first - upper_left.first,
                      lower_right.second - upper_left.second);
}

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
    this->type = (ANALYSIS_TYPE)json["analysis"].toInt();
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
