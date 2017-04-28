#include "AnalysisMethod.h"
#include <qdebug.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"

/**
 * @brief AnalysisMethod::set_include_exclude_area
 * Sets an exlusion frame that will be used to exclude detections in a specific area of each frame.
 * @param points for the polygon that defines the exclusion area.
 */
void AnalysisMethod::set_include_exclude_area(std::vector<cv::Point> points, bool exclude_polygon) {
    if (!capture.isOpened())
        return;

    do_exclusion = true;
    cv::Scalar poly_scalar = cv::Scalar(255);
    cv::Scalar background_scalar = cv::Scalar(0);

    if (exclude_polygon) {
        poly_scalar = cv::Scalar(0);
        background_scalar = cv::Scalar(255);
    }

    cv::Mat img(int(capture.get(cv::CAP_PROP_FRAME_HEIGHT)),int(capture.get(cv::CAP_PROP_FRAME_WIDTH)),CV_8UC1,background_scalar);
    cv::Point* rook_points[1];
    rook_points[0] = &points[0];
    const cv::Point* ppt[1] = {rook_points[0]};
    int npt[1] = {points.size()};
    cv::fillPoly(img, ppt, npt, 1, poly_scalar);
    exclude_frame = img;
}



/**
 * @brief AnalysisMethod::sample_current_frame
 * Checks if the current frame is to be analysed.
 * @return true if the current frame should be analysed.
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
    if (!capture.isOpened()) {
        return m_analysis;
    }

    num_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
    POI* m_POI = new POI();
    while(!aborted && capture.read(frame)) {
        // do frame analysis
        if (sample_current_frame()) {
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
            // TODO do pause stuff
            paused = false;
        }
        emit send_progress(get_progress());
        ++current_frame;
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
