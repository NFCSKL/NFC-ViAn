#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include <opencv2/core/core.hpp>

class ImageGenerator {
    cv::Mat m_frame;
    std::string m_path;
    enum extensions {PNG, TIFF};
public:
    ImageGenerator(cv::Mat frame, std::string path);
    ~ImageGenerator();

    std::string create_thumbnail(std::string name);
    std::string create_tiff(std::string name);

    static const unsigned int THUMBNAIL_SIZE;
private:
    void export_image(std::string s_path, int ext, unsigned int size = 0, bool keep_aspect_ratio = true);
    bool create_directory(std::string path);
};


#endif // IMAGEGENERATOR_H
