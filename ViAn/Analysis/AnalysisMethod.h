#ifndef ANALYSISMETHOD_H
#define ANALYSISMETHOD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "Filehandler/analysis.h"


class AnalysisMethod : public QThread {
    Q_OBJECT
public:
    //Analysis();
    bool load_video();
    void abort_analysis();
    void pause_analysis();

    virtual void setup_analysis() = 0;
    virtual std::vector<OOI> analyse_frame() = 0;
    Analysis run_analysis();
    void set_exclude_area(std::vector<cv::Point> points);
    bool sample_current_frame();

private:
    int prev_detection_frame = -1;
    bool detecting = false;
    bool paused = false;            // Control states
    bool aborted = false;
    Analysis m_analysis;

protected:
    int num_frames = -1;
    unsigned int sample_freq = 5;
    unsigned int current_frame = 0; // The current frame number
    cv::VideoCapture capture;       // Video source
    cv::Mat frame, exclude_frame;                  // The frame fetched last


};

#endif // ANALYSISMETHOD_H
