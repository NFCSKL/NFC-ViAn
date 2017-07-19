#include "poi.h"

/**
 * @brief POI::POI
 */
POI::POI(){
}

/**
 * @brief POI::add_detections
 * Adds OOIs for a specific frame in a POI.
 * @param frame_num
 * @param detections
 */
void POI::add_detections(int frame_num, std::vector<OOI> detections) {    
    if (m_start_frame == -1)
        m_start_frame = frame_num;
    OOIs[frame_num] = detections;
}

/**
 * @brief POI::set_end_frame
 * Sets the end frame of the POI.
 * @param frame_num
 */
void POI::set_end_frame(int frame_num) {
    m_end_frame = frame_num;
}

bool POI::is_in_POI(int frame_num) {
    return frame_num >= m_start_frame && frame_num <= m_end_frame;
}

/**
 * @brief POI::at_edge
 * Checks if frame_num is the next or previous frame of the POI
 * if it is, add it
 * @param frame_num
 * @return
 */
bool POI::at_edge(int frame_num) {
    if (frame_num == m_start_frame-1) {
        m_start_frame = frame_num;
        return true;
    } else if (frame_num == m_end_frame+1) {
        m_end_frame = frame_num;
        return true;
    }
    return false;
}

/**
 * @brief POI::read
 * Reads POI from json format.
 * @param json
 */
void POI::read(const QJsonObject& json) {
    this->m_start_frame = json["start"].toInt();
    this->m_end_frame = json["end"].toInt();
    for(int i = m_start_frame; i != m_end_frame; i++){
        QJsonArray json_frame_OOIs = json[QString::number(i)].toArray();
        std::vector<OOI> oois;
        for(int j = 0; j != json_frame_OOIs.size(); j++){
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
    json["start"] = m_start_frame;
    json["end"] = m_end_frame;
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
 * @brief Analysis::get_detections_on_frame
 * Returns all detections on a specified frame in the analysed video.
 * @param frame_num
 * @return
 */
std::vector<cv::Rect> POI::get_detections_on_frame(int frame_num) {
    std::vector<cv::Rect> rects = {};
    if(in_interval((frame_num))){
        for(OOI ooi: OOIs[frame_num]){
            rects.push_back(ooi.get_rect());
        }
    }
    return rects;
}

POI::POI(int start_frame, int end_frame)
{
    m_start_frame = start_frame;
    m_end_frame = end_frame;
}

bool POI::in_interval(int frame)
{
   return frame >= m_start_frame && frame <= m_end_frame;
}

int POI::getStart() const
{
    return m_start_frame;
}

int POI::getEnd() const
{
    return m_end_frame;
}

std::pair<int, int> POI::getInterval()
{
    return std::make_pair(m_start_frame, m_end_frame);
}


