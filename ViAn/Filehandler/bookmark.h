#ifndef BOOKMARK_H
#define BOOKMARK_H
#include <QJsonObject>
#include <QString>
#include <QImage>
#include <QImageWriter>
#include <QFile>
#include <string>
#include <iostream>
#include "saveable.h"
#include "video.h"
/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class Bookmark : Saveable{
public:
    Bookmark(int frame_nbr, QImage frame, QString video_file_name, QString dir_path, QString string);
    Bookmark();
    int get_frame_number();
    QImage get_frame();
    QString get_file_path();
    QString get_description();
    void set_description(QString text);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void export_frame();
    void create_file_path();
    void remove_exported_image();

private:
    QImage frame;           // Frame of the bookmark
    int frame_number;       // Frame at which the bookmark was taken
    QString video_file_name;// Name of the video file.
    QString dir_path;       // Path to the directory for the bookmarks
    QString description;    // Description for the bookmark, given by user
    // Note that this variable can be altered when the bookmark is exported.
    QString file_path;      // File path to the frame image associated with the bookmark
};

#endif // BOOKMARK_H
