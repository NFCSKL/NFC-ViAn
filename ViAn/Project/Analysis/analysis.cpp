#include "analysis.h"
#include <QDebug>

Analysis::~Analysis() {
    for (auto it = m_intervals.begin(); it != m_intervals.end(); ++it){
        delete *it;
    }
    m_intervals.clear();
}

/**
 * @brief Analysis::read
 * Reads analysis from json format.
 * @param json
 */
void Analysis::read(const QJsonObject &json){
    AnalysisSettings* new_settings = new AnalysisSettings();
    int height = json["bounding box height"].toInt();
    int width = json["bounding box width"].toInt();
    int y = json["bounding box y"].toInt();
    int x = json["bounding box x"].toInt();
    new_settings->bounding_box = cv::Rect(x, y, width, height);

    new_settings->interval = std::make_pair(json["interval start"].toInt(), json["interval end"].toInt());
    settings = new_settings;


    this->type = (ANALYSIS_TYPE)json["type"].toInt();
    this->m_name = json["name"].toString().toStdString();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        POI* poi = new POI();
        poi->read(json_poi);
        this->add_interval(poi);
    }
    m_unsaved_changes = false;
}

/**
 * @brief Analysis::write
 * Writes analysis to json format.
 * @param json
 */
void Analysis::write(QJsonObject &json){

    // TODO Fix save it in the analysis file
    json["bounding box height"] = settings->bounding_box.height;
    json["bounding box width"] = settings->bounding_box.width;
    json["bounding box y"] = settings->bounding_box.y;
    json["bounding box x"] = settings->bounding_box.x;
    json["interval start"] = settings->interval.first;
    json["interval end"] = settings->interval.second;
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
    m_unsaved_changes = false;
}

ANALYSIS_TYPE Analysis::get_type() const {
    return type;
}

std::vector<cv::Rect> Analysis::get_detections_on_frame(int frame_num) {
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

