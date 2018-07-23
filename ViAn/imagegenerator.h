#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <opencv2/core/core.hpp>

/**
 * @brief The ImageGenerator class
 * Class used to export single images
 * and create thumbnails
 */
class ImageGenerator {
    cv::Mat m_frame;
    std::string m_path;
    enum extensions {PNG, TIFF};
public:
    ImageGenerator(cv::Mat frame, std::string path);
    ~ImageGenerator();

    std::string create_thumbnail(std::string name);
    std::string create_tiff(std::string name);
    std::string create_bookmark(std::string name);
    std::string add_serial_number(std::string name, std::string file_end);

    static const unsigned int THUMBNAIL_SIZE;
    static const std::string THUMBNAIL_FOLDER;
private:
    std::string export_image(std::string s_path, int ext, const unsigned int size = 0, bool keep_aspect_ratio = true);
    bool create_directory(std::string path);
};


#endif // IMAGEGENERATOR_H
