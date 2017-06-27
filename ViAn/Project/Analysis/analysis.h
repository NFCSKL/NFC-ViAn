#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"
#include "poi.h"
#include "ooi.h"

enum ANALYSIS_TYPE {MOTION_DETECTION = 0, FACIAL_DETECTION = 1};

const std::vector<std::string> ANALYSIS_NAMES = {"Motion detection", "Facial detection"};
const std::map<std::string, ANALYSIS_TYPE> ANALYSIS_NAMES_TYPE_MAP = {std::make_pair("Motion detection",MOTION_DETECTION),
                                                                     std::make_pair("Facial detection",FACIAL_DETECTION)};
class Analysis : public Saveable {
    friend class AnalysisMeta;
    std::string name;

public:
    ANALYSIS_TYPE type;
    Analysis();
    Analysis(const Analysis &obj);
    ~Analysis();    

    void add_POI(POI POI);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);

    std::vector<POI> POIs;
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    void set_name(const std::string &name);
    std::string getName() const;
};

#endif // ANALYSIS_H
