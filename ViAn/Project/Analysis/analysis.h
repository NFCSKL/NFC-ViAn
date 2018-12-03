#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "basicanalysis.h"

#include "opencv2/core/core.hpp"

#include <QJsonObject>

/**
 * @brief The Analysis class
 *
 */
class Analysis : public BasicAnalysis {
    friend class AnalysisProxy;
public:
    ANALYSIS_TYPE type;
public:
    ~Analysis() override;
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject& json) override;
    virtual ANALYSIS_TYPE get_type() const override;
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    QString get_name() const;

};

#endif // ANALYSIS_H
