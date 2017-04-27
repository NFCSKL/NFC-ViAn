#include "AnalysisController.h"
#include <vector>
#include <QApplication>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Filehandler/analysis.h"
#include "Analysis/MotionDetection.h"

AnalysisController::AnalysisController(std::string file_path, ANALYSIS_TYPE type, QObject* parent) : QThread(parent) {
    switch (type) {
    case MOTION:
        method = new MotionDetection(file_path);
        break;
    default:
        method = new MotionDetection(file_path);
        break;
    }
}

/**
 * @brief Analysis::run
 * TODO add comment
 */
void AnalysisController::run() {
    std::vector<cv::Point> points;
    std::cout << "Analysis thread started" << std::endl;
    method->setup_analysis();
    Analysis analysis = method->run_analysis();
    emit save_analysis(analysis);
    std::cout << "Exiting analysis thread " << std::endl;
}

/**
 * @brief AnalysisController::on_start
 * Flips the paused bool to false.
 * Starts or resumes the analysis
 */
void AnalysisController::on_start() {
}

/**
 * @brief AnalysisController::on_pause
 * Flips the paused bool to true.
 * Pauses the analysis
 */
void AnalysisController::on_pause() {
    method->pause_analysis();
}

/**
 * @brief AnalysisController::on_abort
 * Flips the aborted bool to true.
 * Aborts the analysis
 */
void AnalysisController::on_abort() {
    method->abort_analysis();
}
