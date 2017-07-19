#ifndef POI_H
#define POI_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"
#include "ooi.h"
class POI {
    std::map<int,std::vector<OOI>> OOIs = {};
    int m_start_frame = -1;
    int m_end_frame = -1;
public:
    POI();    
    void add_detections(int frame_num, std::vector<OOI> detections);
    void set_end_frame(int frame_num);
    bool is_in_POI(int frame_num);
    bool at_edge(int frame_num);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    friend class Analysis;
public:
    POI(int start_frame, int end_frame);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    bool in_interval(int frame);
    int getStart() const;
    int getEnd() const;
    std::pair<int,int> getInterval();
};


#endif // POI_H
