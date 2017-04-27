#include "AnalysisMethod.h"
#include <qdebug.h>
#include "opencv2/highgui/highgui.hpp"

void AnalysisMethod::set_exclude_area(std::vector<cv::Point> points) {
    cv::Mat img(int(capture.get(cv::CAP_PROP_FRAME_HEIGHT)),int(capture.get(cv::CAP_PROP_FRAME_WIDTH)),CV_8UC1,cv::Scalar(255));
    exclude_frame = img;
    //cv::floodFill(exclude_frame,cv::Point(0,0),Scalar())

}

/**
 * @brief AnalysisMethod::sample_current_frame
 * @return
 * TODO
 */
bool AnalysisMethod::sample_current_frame() {
    return current_frame % sample_freq == 0;
}

Analysis AnalysisMethod::run_analysis() {
    if (capture.isOpened()) {
        num_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
        POI* m_POI = new POI();
        while(!aborted && capture.read(frame)) {
            // do frame analysis
            if (true || sample_current_frame()) {
                std::vector<OOI> detections = analyse_frame();

                if (detections.empty() && detecting) {
                    qDebug() << "Stopping detection";
                    m_POI->set_end_frame(current_frame - 1);
                    m_analysis.add_POI(*m_POI);
                    m_POI = new POI();
                    detecting = false;
                } else if (!detections.empty()) {
                    qDebug() << "Detecting";
                    detecting = true;
                    m_POI->add_detections(current_frame, detections);
                }

                if (current_frame == num_frames && detecting) {
                    m_POI->set_end_frame(current_frame);
                }
            }

            if (paused) {
                // do pause stuff
                paused = false;
            }
            current_frame++;
        }
    }
    capture.release();
    return m_analysis;
}

void AnalysisMethod::abort_analysis() {
    aborted = true;
    paused = false;
}

void AnalysisMethod::pause_analysis() {
    paused = true;
}
