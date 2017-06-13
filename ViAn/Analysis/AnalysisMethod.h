#ifndef ANALYSISMETHOD_H
#define ANALYSISMETHOD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "Filehandler/Analysis/analysis.h"

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

    Analysis run_analysis();
    int get_progress();

private:
    int prev_detection_frame = -1;
    bool detecting = false;
    bool paused = false;            // Control states
    bool aborted = false;

protected:
    const int FULL_HD_WIDTH = 1920;
    const int FULL_HD_HEIGHT = 1080;

    int num_frames = -1;
    unsigned int sample_freq = 5;
    int current_frame_index = 0;    // The current frame number

    int scaled_width = -1;
    int scaled_height = -1;
    float scaling_ratio = 1.0;

    cv::VideoCapture capture;       // Video source
    cv::Mat frame, exclude_frame;   // The frame fetched last
    Analysis m_analysis;

    bool do_exclusion = false;
    bool scaling_needed = false;
    bool m_exclude_polygon;
    std::vector<cv::Point> include_exclude_poly;

    void calculate_scaling_factor();
    void scale_frame();

signals:
    void send_progress(int progress);


};

#endif // ANALYSISMETHOD_H
