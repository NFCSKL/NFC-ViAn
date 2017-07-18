#include "imageexporter.h"

#include <QDebug>
#include <QCoreApplication>

/**
 * @brief ImageExporter::ImageExporter
 * The ImageExporter is meant to be used to export an interval
 * of frames from a video file. It is meant to be run on its own thread.
 * @param interval  :   end points of the interval to be exported
 * @param file_path :   path to the video to export images from
 */
ImageExporter::ImageExporter(const std::pair<int, int>& interval, const std::string& file_path, const std::string& export_path) {
    set_interval(interval);
    m_file_path = file_path;
    m_export_path = export_path;
}

ImageExporter::ImageExporter(){
}

void ImageExporter::set_interval(const std::pair<int, int> &interval){
    m_export_interval = (interval.first < interval.second) ?
                interval : std::make_pair(interval.second, interval.first);
}

void ImageExporter::set_file_path(const std::string &file_path){
    m_file_path = file_path;
}

void ImageExporter::set_export_path(const std::string &export_path){
    m_export_path = export_path;
}

std::pair<int, int> ImageExporter::get_interval(){
    return m_export_interval;
}

/**
 * @brief ImageExporter::export_images
 * Exports all images in the given interval.
 * This can take a long time and should as such be done in a seperate thread.
 */
void ImageExporter::export_images() {
    cv::VideoCapture capture(m_file_path);
    if (!capture.isOpened()) {
        return;
    }

    // Get number of digits in the last frame number
    int max_digits = Utility().number_of_digits(capture.get(CV_CAP_PROP_FRAME_COUNT));
    int current_frame = m_export_interval.first;
    int progress = 0;
    std::string padded_number = "";

    cv::Mat frame;
    capture.set(CV_CAP_PROP_POS_FRAMES, m_export_interval.first);
    while (capture.read(frame) && (m_export_interval.first + progress) <= m_export_interval.second && !m_aborted) {
        padded_number = Utility().zfill(std::to_string(current_frame), max_digits);
        cv::imwrite(m_export_path + padded_number + ".tiff", frame);
        current_frame++;
        emit update_progress(++progress);
        QCoreApplication::processEvents(); // Process thread event loop. Needed for abort flag to update
    }

    // According to the qt docs the progressdialog should be set to max when done.
    emit update_progress(m_export_interval.second - m_export_interval.first + 1);

    frame.release();
    capture.release();
    emit finished_msg("Exported " + QString::number(progress) + " images");
    emit finished();
}

void ImageExporter::abort() {
    m_aborted = true;
}
