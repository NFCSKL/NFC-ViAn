#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include "video.h"
#include <vector>
#include "utility.h"
#include <QJsonArray>

class ImageSequence : public Video {
private:
    std::vector<std::string> m_images{};
    std::string seq_path{}; // Path to the folder containing the images
public:
    ImageSequence(const std::string& name);
    ImageSequence(const std::string& name, const std::vector<std::string>& images);

    std::vector<std::string> get_images();
    std::vector<std::string> get_image_names();
    std::string get_pattern_name();

    void add_image(const std::string &image_path, const int& index=-1);
    void reorder_elem(const int& from, const int& to);
    int length();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
};

#endif // IMAGESEQUENCE_H
