#include "imagegenerator.h"
#include <QDebug>
#include <iostream>
#include <QDir>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

const unsigned int ImageGenerator::THUMBNAIL_SIZE = 80;

ImageGenerator::ImageGenerator(cv::Mat frame, std::string proj_path){
    m_frame = frame.clone();
    m_path = proj_path;
}

ImageGenerator::~ImageGenerator() {
    m_frame.release();
}

std::string ImageGenerator::create_thumbnail(std::string name) {
    std::string save_path = m_path + "/_thumbnails/";
    if (!create_directory(save_path)) return "";
    export_image(save_path + name, PNG, ImageGenerator::THUMBNAIL_SIZE);
    return save_path + name + ".png";
}

void ImageGenerator::create_tiff(std::string name) {
    std::string save_path = m_path + "/Stills/";
    if (!create_directory(save_path)) return;
    export_image(save_path + name, TIFF);
}

/**
 * @brief ImageGenerator::create_directory
 * Creates the full directory path.
 * @return true if the path exists/has been created
 */
bool ImageGenerator::create_directory(std::string path){
    const QString q_path = QString::fromStdString(path);
    bool success = QDir().exists(q_path);
    if (!success) {
        success = QDir().mkpath(q_path);
    }
    return success;
}

void ImageGenerator::export_image(std::string s_path, int ext, const unsigned int size, bool keep_aspect_ratio) {
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

    // Extension
    switch (ext) {
        case PNG:
            s_path += ".png";
            break;
        case TIFF:
            s_path += ".tiff";
            break;
        default:
            s_path += ".png";
            break;
    }
    cv::imwrite(s_path, tmp);
    tmp.release();
}


