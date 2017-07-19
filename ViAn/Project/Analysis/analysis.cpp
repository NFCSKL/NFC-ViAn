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
Analysis::Analysis(const Analysis &obj) : type(obj.type) {
    m_intervals = obj.m_intervals;
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
void Analysis::add_interval(POI *poi){
    m_intervals.insert(poi);
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
        this->add_interval(poi);
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
    for(auto it = this->m_intervals.begin(); it != this->m_intervals.end(); it++){
        QJsonObject json_POI;
        POI* p = *it;
        p->write(json_POI);
        json_POIs.append(json_POI);
    }
    json["POI:s"] = json_POIs;
}

std::vector<cv::Rect> Analysis::get_detections_on_frame(int frame_num)
{
    for (auto it = m_intervals.begin(); it != m_intervals.end(); it++) {
        POI* ai = static_cast<POI*>(*it);
        return ai->get_detections_on_frame(frame_num);
    }
}

