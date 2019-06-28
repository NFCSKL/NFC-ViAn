#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include "constants.h"
#include "opencv2/core/core.hpp"
#include <QString>

/**
 * @brief The ImageGenerator class
 * Class used to export single images
 * and create thumbnails
 */
class ImageGenerator {
    cv::Mat m_frame;
    QString m_path;
    enum extensions {PNG, TIFF};
public:
    ImageGenerator(cv::Mat frame, QString path);
    ~ImageGenerator();

    QString create_thumbnail(QString name, unsigned int size = Constants::THUMBNAIL_SIZE);
    QString create_tiff(QString name);
    QString create_bookmark(QString name);

private:
    QString export_image(QString s_path, int ext, const unsigned int size = 0, bool keep_aspect_ratio = true);
    bool create_directory(QString path);
};


#endif // IMAGEGENERATOR_H
