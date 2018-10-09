#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include "video.h"
#include <vector>
#include "utility.h"
#include <QJsonArray>
#include "project.h"

class ImageSequence : public Video {
private:
    std::vector<std::string> m_images{};
    std::string seq_path{}; // Path to the folder containing the images
    int m_type = 1;
public:
    ImageSequence(const std::string& name, int seq_type = 1);
    ImageSequence(const std::string& name, const std::vector<std::string>& images, int seq_type = 1);

    std::vector<std::string> get_images();
    std::vector<std::string> get_image_names();
    std::string get_pattern_name();
    int get_type();

    void add_image(const std::string &image_path, const int& index=-1);
    void reorder_elem(const int& from, const int& to);
    void reset_root_dir(const std::string &dir);
    int length();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
};

#endif // IMAGESEQUENCE_H
