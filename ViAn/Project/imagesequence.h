#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include "video.h"
#include <QJsonObject>
#include <map>
#include <vector>

class ImageSequence : public Video {
private:
    // Stores the original file paths belonging to the hashes (hash : path)
    std::map<QString, QString> m_original_images;

    // Stores the saved order of the images (hash : index)
    std::map<QString, int> m_saved_order;

    // Stores the unsaved order of the images (hash : index);
    std::map<QString, int> m_unsaved_order;

    // Contains the temporary names of files that have been removed from the sequence but still exists on disc
    // waiting to be remove or restored on save/not save
    std::map<QString, QString> m_temp_names;

    // Path to the folder containing the images
    QString m_seq_path{};

    void update();
    static const int INDEX_LENGTH{10};
    bool is_new{false};

public:
    ImageSequence(const QString &name, VIDEO_TYPE seq_type = VIDEO_SEQUENCE);
    ImageSequence(const QString& name, const std::vector<QString>& images,
                  const std::vector<QString>& checksums, VIDEO_TYPE seq_type = VIDEO_SEQUENCE);

    std::map<QString, QString> get_paths() const;

    QString get_search_path() const;
    QString get_pattern_name();
    std::map<QString, int> get_saved_order() const;
    std::map<QString, int> get_unsaved_order() const;
    std::vector<QString> get_unsaved_hashes() const;
    int get_index_of_hash(const QString& hash) const;
    QString get_original_name_from_hash(const QString& hash) const;
    QString get_original_name_from_index(const int& index) const;

    bool never_saved() const;


    void set_remove(const bool& remove);
    bool remove_image_with_hash(const QString& hash);
    bool remove_image_from_disc(const QString& image_name) const;
    bool rename_image_on_disc(const QString& old_name, const QString& new_name) const;
    void add_image(const QString &image_path, const int& index=-1); //UNUSED TODO REMOVE?
    void reorder_elem(const int& from, const int& to); //UNUSED TODO REMOVE?
    int length();

    void revert();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
    void reset_root_dir(const QString &dir);
};

#endif // IMAGESEQUENCE_H
