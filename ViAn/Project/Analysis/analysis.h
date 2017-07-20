#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include <set>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"
#include "poi.h"
#include "detectionbox.h"
#include "basicanalysis.h"

class Analysis : public BasicAnalysis {
    friend class AnalysisMeta;
public:
    int type = MOTION_DETECTION;
public:

    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    void set_name(const std::string &name);
    std::string get_name() const;

};

#endif // ANALYSIS_H
