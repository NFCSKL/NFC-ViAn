#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include "video.h"
#include <vector>
#include "utility.h"
#include <QJsonArray>

class ImageSequence : public Video {
private:
    // Stores the original file paths belonging to the hashes (hash : path)
    std::map<std::string, std::string> m_original_images;

    // Stores the saved order of the images (hash : index)
    std::map<std::string, int> m_saved_order;

    // Stores the unsaved order of the images (hash : index);
    std::map<std::string, int> m_unsaved_order;

    // Contains the temporary names of files that have been removed from the sequence but still exists on disc
    // waiting to be remove or restored on save/not save
    std::map<std::string, std::string> m_temp_names;

    // Path to the folder containing the images
    std::string m_seq_path{};

    void on_save();
public:
    ImageSequence(const std::string& name);
    ImageSequence(const std::string& name, const std::vector<std::string>& images, const std::vector<std::string>& checksums);

    std::string get_search_path() const;
    std::string get_pattern_name();
    std::map<std::string, int> get_saved_order() const;
    std::map<std::string, int> get_unsaved_order() const;
    std::vector<std::string> get_unsaved_hashes() const;
    int get_index_of_hash(const std::string& hash) const;
    std::string get_original_name_from_hash(const std::string& hash) const;

    bool remove_image_with_hash(const std::string& hash);
    void add_image(const std::string &image_path, const int& index=-1); //UNUSED TODO REMOVE?
    void reorder_elem(const int& from, const int& to); //UNUSED TODO REMOVE?
    int length();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
};

#endif // IMAGESEQUENCE_H
