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
    void abort_analysis();
    void pause_analysis();
    void set_include_exclude_area(std::vector<cv::Point> points, bool exclude_polygon);

    virtual void setup_analysis() = 0;
    virtual std::vector<OOI> analyse_frame() = 0;

    bool sample_current_frame();
    bool load_video();

    int get_progress();
    Analysis run_analysis();

private:
    int prev_detection_frame = -1;
    bool detecting = false;
    bool paused = false;            // Control states
    bool aborted = false;

protected:
    int num_frames = -1;
    unsigned int sample_freq = 5;
    unsigned int current_frame = 0; // The current frame number

    cv::VideoCapture capture;       // Video source
    cv::Mat frame, exclude_frame;   // The frame fetched last
    Analysis m_analysis;

    bool do_exclusion = false;
    bool m_exclude_polygon;
    std::vector<cv::Point> include_exclude_poly;

signals:
    void send_progress(int progress);


};

#endif // ANALYSISMETHOD_H
