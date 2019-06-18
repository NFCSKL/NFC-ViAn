#ifndef POI_H
#define POI_H

#include "analysisinterval.h"
#include "detectionbox.h"

#include "opencv2/core/core.hpp"

#include <QJsonObject>

#include <map>

class POI : public AnalysisInterval{    
    std::map<int,std::vector<DetectionBox>> OOIs = {};
public:
    POI();
    ~POI() override;
    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;

    void add_detections(int frame_num, std::vector<DetectionBox> detections);
    void set_end_frame(int frame_num);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    std::vector<DetectionBox> get_detectionbox_on_frame(int frame_num);
    friend class Analysis;

};


#endif // POI_H
