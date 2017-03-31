#include "AnalysisMethod.h"
#include <iostream>

#include "opencv2/highgui/highgui.hpp"

/**
 * @brief Analysis::run
 * TODO add comment
 */
void AnalysisMethod::run() {
    std::cout << "Analysis thread started" << std::endl;
    setup_analysis();
    if (capture.isOpened()) {
        while(!aborted && capture.read(frame)) {
            // do frame analysis
            do_analysis();
            cv::waitKey(10);

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
