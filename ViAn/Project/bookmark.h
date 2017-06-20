#ifndef BOOKMARK_H
#define BOOKMARK_H
#include <QJsonObject>
#include <QString>
#include <QImage>
#include <QImageWriter>
#include <QFile>
#include <string>
#include <iostream>
#include "Filehandler/saveable.h"
#include "video.h"
/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class Bookmark : public Saveable{
public:
    Bookmark(const std::string& file_path, const std::string& text, const int& frame_nbr);
    Bookmark();
    int get_time();
    int get_frame_number();
    std::string get_description();
    void set_description(const std::string &text);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    Q_DECL_DEPRECATED void export_frame();
    Q_DECL_DEPRECATED void create_file_path();
    Q_DECL_DEPRECATED void remove_exported_image();

private:
    int frame_number = -1;       // Frame at which the bookmark was taken
    int time = -1;               // Time of the bookmark (in millisecs)
    std::string description = "";    // Description for the bookmark, given by user
};

#endif // BOOKMARK_H
