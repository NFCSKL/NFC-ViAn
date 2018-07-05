#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include "video.h"
#include <vector>
#include "utility.h"
#include <QJsonArray>

class ImageSequence : public Video {
private:
    std::vector<std::string> m_images{};
    std::map<std::string, std::string> m_original_images;
    std::map<std::string, int> m_saved_order;
    std::map<std::string, int> m_unsaved_order;
    std::string seq_path{}; // Path to the folder containing the images

    void on_save();
public:
    ImageSequence(const std::string& name);
    ImageSequence(const std::string& name, const std::vector<std::string>& images);

    std::string get_container_path() const;
    std::vector<std::string> get_images();
    std::vector<std::string> get_image_names();
    std::string get_pattern_name();
    std::map<std::string, int> get_saved_order() const;
    std::map<std::string, int> get_unsaved_order() const;
    std::vector<std::string> get_unsaved_hashes() const;

    void add_image(const std::string &image_path, const int& index=-1);
    void reorder_elem(const int& from, const int& to);
    int length();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
};

#endif // IMAGESEQUENCE_H
