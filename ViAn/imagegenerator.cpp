#include "imagegenerator.h"

#include "constants.h"
#include "Project/project.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>
#include <QDir>

ImageGenerator::ImageGenerator(cv::Mat frame, std::string proj_path){
    m_frame = frame.clone();
    m_path = proj_path;
}

ImageGenerator::~ImageGenerator() {
    m_frame.release();
}

std::string ImageGenerator::create_thumbnail(std::string name) {
    std::string save_path = m_path + Constants::THUMBNAIL_FOLDER.toStdString();
    if (!create_directory(save_path)) return "";
    return export_image(save_path + name, PNG, Constants::THUMBNAIL_SIZE);

}

std::string ImageGenerator::create_tiff(std::string name) {
    std::string save_path = m_path + Constants::STILLS_FOLDER.toStdString();
    if (!create_directory(save_path)) return "";    
    return export_image(save_path + name, TIFF);
}


std::string ImageGenerator::create_bookmark(std::string name) {
    std::string save_path = m_path + Constants::BOOKMARK_FOLDER.toStdString();
    if (!create_directory(save_path)) return "";
    return export_image(save_path + name, TIFF);
}
/**
 * @brief ImageGenerator::add_serial_number
 * @param name
 * @param file_end
 * @return
 * Appends (i) to file if i-1 images with same name exist
 */
std::string ImageGenerator::add_serial_number(std::string name, std::string file_end) {
    QString qend = QString::fromStdString(file_end);
    QString qname = QString::fromStdString(name);
    QString res = qname+qend;
    int i = 0;
    if(QFile::exists(res)){
        res = qname + QString("(%1)"+ qend).arg(i);
    }
    while(QFile::exists(res) && ++i){
        res = qname + QString("(%1)"+ qend).arg(i);
    }
    return res.toStdString();
}


/**
 * @brief ImageGenerator::create_directory
 * Creates the full directory path.
 * @return true if the path exists/has been creat§ed
 */
bool ImageGenerator::create_directory(std::string path) {
    const QString q_path = QString::fromStdString(path);
    bool success = QDir().exists(q_path);
    if (!success) {
        success = QDir().mkpath(q_path);
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
std::string ImageGenerator::export_image(std::string requested_path, int ext, const unsigned int size, bool keep_aspect_ratio) {
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
    std::string end;
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
    std::string saved_path = add_serial_number(requested_path, end);
    cv::imwrite(saved_path, tmp);
    tmp.release();
    return saved_path;
}
