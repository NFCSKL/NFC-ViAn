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

class POI : Saveable{
    std::map<int,std::vector<OOI>> OOIs;
public:

    POI();
    int start_frame = -1;
    int end_frame = -1;
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void add_detections(int frame_num, std::vector<OOI> detections);
    void set_end_frame(int frame_num);
    bool is_in_POI(int frame_num);
    bool at_edge(int frame_num);
    friend class Analysis;
};


#endif // POI_H
