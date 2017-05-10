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
    calculate_scaling_factor();
    num_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);
    POI* m_POI = new POI();
    while(!aborted && capture.read(frame)) {
        // do frame analysis
        if (sample_current_frame()) {
            if (scaling_needed)
                scale_frame();

            std::vector<OOI> detections = analyse_frame();

            if (detections.empty() && detecting) {
                m_POI->set_end_frame(current_frame - 1);
                m_analysis.add_POI(*m_POI);
                m_POI = new POI();
                detecting = false;
            } else if (!detections.empty()) {
                detecting = true;
                if (scaling_needed) {
                    for (OOI detection : detections) {
                        detection.scale_coordinates(1.0/scaling_ratio);
                    }
                }
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

void AnalysisMethod::calculate_scaling_factor() {
    int video_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int video_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    float height_ratio = float(FULL_HD_HEIGHT)/float(video_height);
    float width_ratio = float(FULL_HD_WIDTH)/float(video_width);
    if (height_ratio >= 1 && width_ratio >= 1) return;

    scaling_needed = true;
    //This statement ensures that the original aspect ratio of the video is kept when scaling
    if (width_ratio >= height_ratio) {
        scaling_ratio = height_ratio;
        scaled_width = int(video_width * scaling_ratio);
        scaled_height = FULL_HD_HEIGHT;
    } else {
        scaling_ratio = width_ratio;
        scaled_width = FULL_HD_WIDTH;
        scaled_height = int(video_height * scaling_ratio);
    }
}

void AnalysisMethod::scale_frame() {

    cv::Size size(scaled_width,scaled_height);
    cv::Mat dst(size,frame.type());
    cv::resize(frame,dst,size); //resize frame
    frame = dst;
}
