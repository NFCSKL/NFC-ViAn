#include "AnalysisMethod.h"
#include <qdebug.h>
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief AnalysisMethod::set_exclude_area
 * Sets an exlusion frame that will be used to exclude detections in a specific area of each frame.
 * @param points for the polygon that defines the exclusion area.
 */
void AnalysisMethod::set_exclude_area(std::vector<cv::Point> points) {
    cv::Mat img(int(capture.get(cv::CAP_PROP_FRAME_HEIGHT)),int(capture.get(cv::CAP_PROP_FRAME_WIDTH)),CV_8UC1,cv::Scalar(255));
    exclude_frame = img;
    //cv::floodFill(exclude_frame,cv::Point(0,0),Scalar())

}

/**
 * @brief AnalysisMethod::sample_current_frame
 * @return true if the current frame should be analysed.
 * TODO
 */
bool AnalysisMethod::sample_current_frame() {
    return current_frame % sample_freq == 0;
}

/**
 * @brief AnalysisMethod::run_analysis
 * This is the main loop method for doing an analysis.
 * @return all detections from the performed analysis.
 */
Analysis AnalysisMethod::run_analysis() {
    if (capture.isOpened()) {
        num_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
        POI* m_POI = new POI();
        while(!aborted && capture.read(frame)) {
            // do frame analysis
            if (true || sample_current_frame()) {
                std::vector<OOI> detections = analyse_frame();

                if (detections.empty() && detecting) {
                    m_POI->set_end_frame(current_frame - 1);
                    m_analysis.add_POI(*m_POI);
                    m_POI = new POI();
                    detecting = false;
                } else if (!detections.empty()) {
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
            emit send_progress(get_progress());
            current_frame++;
        }
    }
    capture.release();
    return m_analysis;
}

/**
 * @brief AnalysisMethod::get_progress
 * @return Progression of analysis in whole percent.
 */
int AnalysisMethod::get_progress() {
    return current_frame*100/num_frames;
}

/**
 * @brief AnalysisMethod::abort_analysis
 * Sets the necessary bools to abort an analysis.
 */
void AnalysisMethod::abort_analysis() {
    aborted = true;
    paused = false;
}

/**
 * @brief AnalysisMethod::pause_analysis
 * Sets the necessary bool to pause an analysis.
 */
void AnalysisMethod::pause_analysis() {
    paused = true;
}
