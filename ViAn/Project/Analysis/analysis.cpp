#include "analysis.h"
/**
 * @brief Analysis::read
 * Reads analysis from json format.
 * @param json
 */
void Analysis::read(const QJsonObject &json){
    this->type = (ANALYSIS_TYPE)json["type"].toInt();
    this->m_name = json["name"].toString().toStdString();
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
    json["name"] = QString::fromStdString(this->m_name);
    QJsonArray json_POIs;
    for(auto it = this->m_intervals.begin(); it != this->m_intervals.end(); it++){
        QJsonObject json_POI;
        POI* p = dynamic_cast<POI*>(*it);
        p->write(json_POI);
        json_POIs.append(json_POI);
    }
    json["POI:s"] = json_POIs;
}

SAVE_TYPE Analysis::get_save_type() const
{
    return DETECTION;
}

ANALYSIS_TYPE Analysis::get_type() const
{
    return type;
}

std::vector<cv::Rect> Analysis::get_detections_on_frame(int frame_num)
{
    std::vector<cv::Rect> res;
    for (auto it = m_intervals.begin(); it != m_intervals.end(); it++) {
        POI* ai = static_cast<POI*>(*it);
        if(ai->in_interval(frame_num)) {
            res = ai->get_detections_on_frame(frame_num);
            break;
        }
    }
    return res;
}

