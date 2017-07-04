#include "imagegenerator.h"
#include <QDebug>
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
    std::string full_path = m_path + "/_thumbnails/"  + name;
    export_image(full_path, PNG, ImageGenerator::THUMBNAIL_SIZE);
    return full_path += ".png";
}

void ImageGenerator::create_tiff(std::string name) {
    std::string full_path = m_path + "/Stills/" + name;
    export_image(full_path, TIFF);
}

void ImageGenerator::export_image(std::string s_path, int ext, const unsigned int size, bool keep_aspect_ratio) {
    cv::Mat tmp = m_frame.clone();
    if (size > 0) {
        // Do resize
        double ratio = 1;
        if (keep_aspect_ratio) ratio = m_frame.cols / static_cast<double>(m_frame.rows);
        resize(m_frame, tmp, cv::Size(ratio * size, size));
        qDebug() << "resized";
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


