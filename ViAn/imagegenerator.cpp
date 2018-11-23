#include "imagegenerator.h"

#include "constants.h"
#include "Project/project.h"
#include "utility.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QDir>

ImageGenerator::ImageGenerator(cv::Mat frame, QString proj_path){
    m_frame = frame.clone();
    m_path = proj_path;
}

ImageGenerator::~ImageGenerator() {
    m_frame.release();
}

QString ImageGenerator::create_thumbnail(QString name) {
    QString save_path = m_path + Constants::THUMBNAIL_FOLDER;
    if (!create_directory(save_path)) return "";
    return export_image(save_path + name, PNG, Constants::THUMBNAIL_SIZE);

}

QString ImageGenerator::create_tiff(QString name) {
    QString save_path = m_path + Constants::STILLS_FOLDER;
    if (!create_directory(save_path)) return "";
    return export_image(save_path + name, TIFF);
}


QString ImageGenerator::create_bookmark(QString name) {
    QString save_path = m_path + Constants::BOOKMARK_FOLDER;
    if (!create_directory(save_path)) return "";
    return export_image(save_path + name, TIFF);
}

/**
 * @brief ImageGenerator::create_directory
 * Creates the full directory path.
 * @return true if the path exists/has been creat§ed
 */
bool ImageGenerator::create_directory(QString path) {
    bool success = QDir().exists(path);
    if (!success) {
        success = QDir().mkpath(path);
    }
    return success;
}

/**
 * @brief ImageGenerator::export_image
 * @param requested_path
 * @param ext
 * @param size
 * @param keep_aspect_ratio
 * @return
 * Export single image to requested path with extension ext
 */
QString ImageGenerator::export_image(QString requested_path, int ext, const unsigned int size, bool keep_aspect_ratio) {
    cv::Mat tmp = m_frame.clone();
    if (size > 0) {
        // Do resize
        double ratio = 1;
        if (keep_aspect_ratio) ratio = m_frame.cols / static_cast<double>(m_frame.rows);
        resize(m_frame, tmp, cv::Size(ratio * size, size));
    }

    // Convert to RGB
    switch (tmp.type()) {
        case CV_8UC1:
            cvtColor(tmp, tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(tmp, tmp, CV_BGR2RGB);
            break;
    }
    QString end;
    // Extension
    switch (ext) {
        case PNG:
            end = ".png";
            break;
        case TIFF:
            end = ".tiff";
            break;
        default:
            end = ".png";
            break;
    }
    QString saved_path = Utility::add_serial_number(requested_path, end);
    cv::imwrite(saved_path.toStdString(), tmp);
    tmp.release();
    return saved_path;
}
