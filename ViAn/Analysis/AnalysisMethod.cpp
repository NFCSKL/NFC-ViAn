#include "AnalysisMethod.h"
#include <qdebug.h>

#include "opencv2/highgui/highgui.hpp"

/**
 * @brief Analysis::run
 * TODO add comment
 */
void AnalysisMethod::run() {
    std::vector<cv::Point> points;
    set_exclude_area(points);
    std::cout << "Analysis thread started" << std::endl;
    setup_analysis();
    if (capture.isOpened()) {
        while(!aborted && capture.read(frame)) {
            // do frame analysis
            if (true || sample_current_frame()) {
                do_analysis();
                cv::waitKey(5);
            }

            if (paused) {
                // do pause stuff
                paused = false;
            }
            current_frame++;
        }
    }
    capture.release();
    std::cout << "Exiting analysis thread " << std::endl;
}

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

/**
 * @brief Analysis::on_start
 * Flips the paused bool to false.
 * Starts or resumes the analysis
 */
void AnalysisMethod::on_start() {
    paused = false;
}

/**
 * @brief Analysis::on_pause
 * Flips the paused bool to true.
 * Pauses the analysis
 */
void AnalysisMethod::on_pause() {
    paused = true;
}

/**
 * @brief Analysis::on_abort
 * Flips the aborted bool to true.
 * Aborts the analysis
 */
void AnalysisMethod::on_abort() {
    aborted = true;
    paused = false;
}
