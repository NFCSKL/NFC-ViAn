#ifndef IMAGESEQUENCE_H
#define IMAGESEQUENCE_H

#include "video.h"

#include <QJsonObject>

class ImageSequence : public Video {
private:
    std::vector<QString> m_images{};
    QString seq_path{}; // Path to the folder containing the images
public:
    ImageSequence(const QString &name);
    ImageSequence(const QString &name, const std::vector<QString> &images);

    std::vector<QString> get_images();
    std::vector<QString> get_image_names();
    QString get_pattern_name();

    void add_image(const QString &image_path, const int& index=-1);
    void reorder_elem(const int& from, const int& to);
    void reset_root_dir(const QString &dir);
    int length();

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) override;
};

#endif // IMAGESEQUENCE_H
