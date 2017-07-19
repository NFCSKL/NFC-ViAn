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
#include "ooi.h"
enum ANALYSIS_TYPE {MOTION_DETECTION = 0, FACIAL_DETECTION = 1, TAG = 2};

const std::vector<std::string> ANALYSIS_NAMES = {"Motion detection", "Facial detection", "Tag"};
const std::map<std::string, ANALYSIS_TYPE> ANALYSIS_NAMES_TYPE_MAP = {std::make_pair("Motion detection",MOTION_DETECTION),
                                                                     std::make_pair("Facial detection",FACIAL_DETECTION),
                                                                     std::make_pair("Tag",TAG)};
class Analysis : public Saveable {
    friend class AnalysisMeta;    
public:
    int type = MOTION_DETECTION;
protected:
    std::string name;
    struct poi_cmp {
        bool operator()(const POI* lhs, const POI* rhs) const {
            return lhs->getEnd() < rhs->getStart();
        }
    };

public:
    std::set<POI*, poi_cmp> m_intervals;
    Analysis();
    Analysis(const Analysis &obj);
    ~Analysis();

    virtual void add_interval(POI *POI);

    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    void set_name(const std::string &name);
    std::string get_name() const;

};

#endif // ANALYSIS_H
