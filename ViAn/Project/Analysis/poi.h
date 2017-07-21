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
#include "detectionbox.h"
#include "analysisinterval.h"
class POI : public AnalysisInterval{    
    std::map<int,std::vector<DetectionBox>> OOIs = {};
public:
    POI();    
    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;

    void add_detections(int frame_num, std::vector<DetectionBox> detections);
    void set_end_frame(int frame_num);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    friend class Analysis;

};


#endif // POI_H
