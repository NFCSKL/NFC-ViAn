#ifndef BOOKMARK_H
#define BOOKMARK_H
#include <QJsonObject>
#include <QString>
#include <QImage>
#include <QImageWriter>
#include <QFileInfo>
#include <string>
#include <iostream>
#include "saveable.h"
/**
 * @brief The Bookmark class
 * Bookmark class is used for storing bookmarks, i.e. user
 * marked points in a video and an associated frame.
 */
class Bookmark : Saveable{
public:
    Bookmark(int frame_nbr, QImage frame, QString dir_path, QString string);
    Bookmark();
    int get_frame_number();
    QImage get_frame();
    QString get_file_path();
    QString get_description();
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
    void export_frame();
    void create_file_path();
private:
    QImage frame;           // Frame of the bookmark
    int frame_number;       // Frame at which the bookmark was taken
    QString dir_path;      // Path to the directory for the bookmarks
    QString file_path;      // File path to the frame image associated with the bookmark
    QString description;    // Description for the bookmark, given by user

};

#endif // BOOKMARK_H
